#include "pch.h"
#include "DBManager.h"
#include "DataManager.h"
#include "Room.h"
#include "Player.h"
#include "Inventory.h"
#include "ObjectUtils.h"
#include "SkillComponent.h"
#include "QuickSlotComponent.h"
#include "QuestComponent.h"
#include "StatComponent.h"
#include "PlayerStatComponent.h"
#include "Quest.h"
#include <iostream>
#include <sstream>

shared_ptr<DBManager> GDBManager = make_shared<DBManager>();
void DBManager::ConnectDB()
{
    driver = sql::mysql::get_mysql_driver_instance();

    try {
        // 데이터베이스 연결
        con = unique_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3306", "root", "vjtlvmtl040912!"));

        if (!con) {
            std::cerr << "Failed to connect to the database" << std::endl;
            return;
        }
        con->setSchema("myrpgdb");
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("SET NAMES 'utf8mb4'");
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
    }
    GetMonsterStatData();
    GetCharacterAttackData();
    GetMonsterAttackData();
    GetWarriorStatData();
    GetRogueStatData();
    GetWizardStatData();
    GetWeaponData();
    GetArmorData();
    GetConsumableData();
    GetWarriorSkillHitData();
    GetRogueSkillHitData();
    GetWizardSkillHitData();
    GetWarriorSkillDamageData();
    GetRogueSkillDamageData();
    GetWizardSkillDamageData();
    GetWarriorSkillMpData();
    GetRogueSkillMpData();
    GetWizardSkillMpData();
    GetQuestInfoData();
    GetQuestTargetData();
    GetQuestRewardData();
	return;
}

void DBManager::HandleLogin(string id, PacketSessionRef session)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM useridtable WHERE userid = ?");
    pstmt->setString(1, id); 
    sql::ResultSet* res = pstmt->executeQuery();

    // ************ 접속한 아이디에 해당하는 "유저 정보"가 있는지 확인
    int64 dbId;
    if (res->next())
    {
        //************ 유저 정보 있음 : 해당 유저의 캐릭터 정보를 불러옴
        dbId = res->getInt64("id");
    }
    else
    {
        // ************ 유저 정보 없음 : 새로운 "유저 정보"를 생성
        sql::PreparedStatement* pstmtInsert = con->prepareStatement("INSERT INTO useridtable (userid, pw) VALUES (?, ?)");
        pstmtInsert->setString(1, id);
        pstmtInsert->setString(2, "0");

        // Execute the INSERT query
        pstmtInsert->execute();

        // Retrieve the auto-generated ID
        sql::PreparedStatement* pstmtId = con->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* resId = pstmtId->executeQuery();

        if (resId->next())
        {
            dbId = resId->getInt64(1); // Assuming the ID is the first column
            std::cout << "New ID: " << dbId << std::endl;
        }

        // Clean up
        delete resId;
        delete pstmtId;
        delete pstmtInsert;
    }
    // Clean up
    delete res;
    delete pstmt;

    HandleLogin2(dbId, session);
}

void DBManager::HandleLogin2(uint64 userId, PacketSessionRef session)
{
    Protocol::S_LOGIN loginPkt;
    // 플레이어가 가진 캐릭터 정보 전부 불러오기
    vector<UserBaseInfo> infoList = GetCharacterBaseInfoList(userId);
    for (int32 i = 0; i < infoList.size(); i++)
    {
        Protocol::BaseCharacterInfo* character = loginPkt.add_characters();
        character->set_object_id(infoList[i].characterId);
        character->set_level(infoList[i].level);
        character->set_name(infoList[i].characterName);
        character->set_type(infoList[i].characterType);
    }
    std::cout << " Login Game - Character Num : " << infoList.size() << std::endl;
    loginPkt.set_userdbid(userId);
    SEND_PACKET(loginPkt);

}

void DBManager::CheckName(PacketSessionRef session, string name)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM usercharactertable WHERE characterName = ?");

    // Bind the value to the placeholder
    pstmt->setString(1, name); // 1-based index

    sql::ResultSet* res = pstmt->executeQuery();
    
    bool isDuplacated = false;
    if (res->next())
    {
        // 결과 값이 있음
        cout << "닉네임 중복 : " << name << endl;
        isDuplacated = true;
    }
    else
    {
        // 결과 값이 없음
        cout << "닉네임 사용 가능 : " << name << endl;
        isDuplacated = false;
    }

    CheckName2(session, name, isDuplacated);
}

void DBManager::CheckName2(PacketSessionRef session, string name, bool isDuplicated)
{
    Protocol::S_CHECKNAME pkt;
    pkt.set_name(name);
    pkt.set_duplication(isDuplicated);

    SEND_PACKET(pkt);
}

void DBManager::CreateCharacter(PacketSessionRef session, Protocol::C_CREATECHARACTER pkt)
{
    uint64 characterId;
    if (CreateNewCharacter(pkt.userid(), pkt.name(), pkt.type(), characterId))
    {
        SetNewCharacterInfo(characterId, pkt.type());
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM questtable WHERE lvl = 1 AND isLocked = 0");
        sql::ResultSet* res = pstmt->executeQuery();

        sql::PreparedStatement* pstmt2 = con->prepareStatement("INSERT INTO availablequesttable (characterId, questId) VALUES (?, ?)");
        while (res->next())
        {
            int32 questId = res->getInt("questId");
            pstmt2->setUInt64(1, characterId);
            pstmt2->setInt(2, questId);
            pstmt2->executeQuery();
        }

        // Clean up
        delete res;
        delete pstmt;
        delete pstmt2;

        HandleLogin2(pkt.userid(), session);
    }
}

void DBManager::EnterPlayer(PlayerRef player, uint64 characterId, RoomRef room)
{
    const UserBaseInfo userBaseInfo = GetUserBaseInfo(characterId);
    const CharacterBaseInfo characterBaseInfo = GetCharacterBaseInfo(characterId);

    room->DoAsync(&Room::HandleEnterPlayer, player, userBaseInfo, characterBaseInfo);
    
}

void DBManager::SetItems(RoomRef room, PlayerRef player)
{
    // 1. DB에서 player가 가진 아이템 전부 검색

    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM itementitytable WHERE ownerId = ?");

    // Bind the value to the placeholder
    pstmt->setUInt64(1, player->GetDbId()); // 1-based index

    // Execute the query
    sql::ResultSet* res = pstmt->executeQuery();

    // Process the result set
    while (res->next())
    {
        // while문 돌면서 UserBaseInfo 생성하고
        uint64 id = res->getUInt64("dataId");
        uint32 tId = res->getUInt64("templateId");
        uint64 oId = res->getInt("ownerId");
        Protocol::EItemType type = (Protocol::EItemType)res->getInt("itemtype");
        Protocol::ESlotState slotState = (Protocol::ESlotState)res->getInt("slotstate");
        int32 count = res->getInt("count");
        int slotIndex = res->getInt("slotindex");
        Protocol::EQuickSlot slot = static_cast<Protocol::EQuickSlot>(res->getInt("quickslot"));

 
        room->DoAsync(&DBManager::AddItem2, type, id, tId, slotIndex, slot, player, count, true);

        if (slotState == Protocol::SLOT_EQUIPPED)
        {
            ItemRef nullItem = nullptr;
            room->DoAsync(&DBManager::EquipItem2, id, player, nullItem);
        }
    }



}

void DBManager::SetSkills(RoomRef room, PlayerRef player)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM characterskillinfotable WHERE characterId = ?");

    // Bind the value to the placeholder
    pstmt->setUInt64(1, player->GetDbId()); // 1-based index

    // Execute the query
    sql::ResultSet* res = pstmt->executeQuery();

    // Process the result set

    vector<SkillInfo> skills;
    while (res->next())
    {
        // while문 돌면서 UserBaseInfo 생성하고
        uint64 cId = player->GetDbId();
        int32 skillId = res->getInt("skillId");
        int32 skillLevel = res->getInt("skillLevel");
        Protocol::EQuickSlot slot = static_cast<Protocol::EQuickSlot>(res->getInt("quickslot"));

        SkillInfo info;
        info.skillId = skillId;
        info.skillLevel = skillLevel;
        info.quickSlot = slot;

        skills.push_back(info);
    }

    room->DoAsync(&DBManager::SetSkills2, room, player, skills);
}

void DBManager::SetSkills2(RoomRef room, PlayerRef player, vector<SkillInfo> skills)
{
    player->GetSkillComponent()->InitWithPlayerStart(skills);
}

void DBManager::AddItem1(Protocol::EItemType itemType, uint32 templateId, Protocol::EQuickSlot slot, PlayerRef owner, RoomRef room, int32 count)
{
    // 여기서 ownerId는 owner의 DB Id이다.
    
    // 1. DB에 아이템 추가하는 쿼리문 실행
    int slotIndex = owner->GetInventory()->FindSlotIndex(itemType);

   
    if (slotIndex == -1)
    {
        cout << "Slot Index : -1" << endl;
        // slotIndex == -1 : 빈 인벤토리를 찾지 못함 -> 클라한테 얻을 수 없었음을 알림
    }
    std::string insert_query = "INSERT INTO itementitytable (templateId, createdata, ownerId, itemtype, slotstate, slotindex, count, quickslot) VALUES (?, NOW(), ?, ?, ?, ?, ?, ?)";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    cout << "Data Binding : " << templateId << ", " << owner->GetDbId() << ", " << (int)itemType << ", " << (int)Protocol::SLOT_INVENTORY << endl;
    pstmt->setInt(1, templateId);
    pstmt->setUInt64(2, owner->GetDbId());
    pstmt->setInt(3, (int)itemType);
    pstmt->setInt(4, (int)Protocol::SLOT_INVENTORY);
    pstmt->setInt(5, slotIndex);
    pstmt->setInt(6, count);
    pstmt->setInt(7, static_cast<int>(slot));

    // 2. 추가한 아이템 데이터를 이용해 GameRoom의 JobQueue에 AddItem2 추가하기.
    pstmt->executeUpdate();

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

   
    if (res->next()) {
        uint64 dataId = res->getUInt64(1); // 마지막으로 삽입된 ID 가져오기
        std::cout << "Last Inserted ID: " << dataId << std::endl;
       

        room->DoAsync(&DBManager::AddItem2, itemType, dataId, templateId, slotIndex, slot, owner, count, true);
    }

    delete stmt;
    delete res;
    delete pstmt;
}

void DBManager::AddItem2(Protocol::EItemType itemType, uint64 dataId, uint32 templateId, int slotIndex, Protocol::EQuickSlot slot, PlayerRef owner, int32 count, bool sendToClient)
{
    // 1. 아이템 객체 생성
    cout << " 새로운 아이템 획득 " << endl;
    ItemRef item = ObjectUtils::CreateItem(itemType, dataId, templateId, count);
    item->SetQuickType(slot);

    // 2. 생성한 아이템을 owner의 인벤토리에 추가하기
    owner->GetInventory()->AddItemToInventory(item, slotIndex, false, sendToClient);
}

void DBManager::RemoveOrDecrement(vector<uint64> ids, vector<uint32> itemCount, PlayerRef player, RoomRef room)
{
    vector<uint64> removeIds;
    vector<uint64> decrementIds;
    vector<uint32> decrementCounts;
    for(int i = 0; i < ids.size(); i++)
    {
        ItemRef item = player->GetInventory()->GetItem(ids[i]);

        int32 leftCount = item->GetCount() - itemCount[i];

        if (leftCount <= 0 || item->GetItemType() == Protocol::ITEM_WEAPON || item->GetItemType() == Protocol::ITEM_ARMOR)
        {
            removeIds.push_back(ids[i]);
        }
        else
        {
            decrementIds.push_back(ids[i]);
            decrementCounts.push_back(leftCount);
        }
    }

    GDBManager->DoAsync(&DBManager::RemoveItem, removeIds, player, room);
    GDBManager->DoAsync(&DBManager::SetItemCount, decrementIds, player, room, decrementCounts);
}

void DBManager::RemoveOrDecrementSingle(uint64 dbId, uint32 itemCount, PlayerRef player, RoomRef room)
{
    ItemRef item = player->GetInventory()->GetItem(dbId);
    int32 leftCount = item->GetCount() - itemCount;

    if (leftCount <= 0 || item->GetItemType() == Protocol::ITEM_WEAPON || item->GetItemType() == Protocol::ITEM_ARMOR)
    {
        GDBManager->DoAsync(&DBManager::RemoveItemSingle, dbId, player, room);
    }
    else
    {
        GDBManager->DoAsync(&DBManager::SetItemCountSingle, dbId, player, room, (uint32)leftCount);
    }  
}

void DBManager::RemoveItemSingle(uint64 dbId, PlayerRef player, RoomRef room)
{
    std::string update_query = "DELETE FROM itementitytable WHERE dataId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

    pstmt->setUInt64(1, dbId);
    pstmt->executeUpdate();

    room->DoAsync(&DBManager::RemoveItemSingle2, dbId, player);
}

void DBManager::RemoveItemSingle2(uint64 dbId, PlayerRef player)
{
    Protocol::S_REMOVEITEM pkt;

    cout << "아이템 제거 : " << dbId << endl;
    player->GetInventory()->RemoveItem(dbId);
    pkt.add_itemid(dbId);

    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);
}

void DBManager::RemoveItem(vector<uint64> ids, PlayerRef player, RoomRef room)
{
    // DB에서 데이터 제거
    for(int i = 0; i < ids.size(); i++)
    {
        std::string update_query = "DELETE FROM itementitytable WHERE dataId = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

        pstmt->setUInt64(1, ids[i]);
        pstmt->executeUpdate();
    }

    room->DoAsync(&DBManager::RemoveItem2, ids, player);
}

void DBManager::RemoveItem2(vector<uint64> ids, PlayerRef player)
{
    // 패킷 전송
    
    Protocol::S_REMOVEITEM pkt;
    
    for(int i = 0; i < ids.size(); i++)
    {
        cout << "아이템 제거 : " << ids[i] << endl;
        player->GetInventory()->RemoveItem(ids[i]);
        pkt.add_itemid(ids[i]);
    }

    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);
}


void DBManager::SetItemCountSingle(uint64 dbId, PlayerRef player, RoomRef room, uint32 itemCount)
{
    std::string update_query = "UPDATE itementitytable SET count = ? WHERE dataId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

    pstmt->setInt(1, itemCount);
    pstmt->setUInt64(2, dbId);
    pstmt->executeUpdate();

    room->DoAsync(&DBManager::SetItemCountSingle2, dbId, player, itemCount);
}

void DBManager::SetItemCountSingle2(uint64 dbId, PlayerRef player, uint32 itemCount)
{
    Protocol::S_SETITEMCOUNT pkt;

    auto item = player->GetInventory()->GetItem(dbId);
    item->SetCount(itemCount, player);
    
    //temp
    

    cout << dbId << "번 아이템 개수 : " << itemCount << endl;
    pkt.add_itemid(dbId);
    pkt.add_itemcount(itemCount);

    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);
}

void DBManager::SetItemCount(vector<uint64> ids, PlayerRef player, RoomRef room, vector<uint32> itemCounts)
{
    for(int i = 0; i < ids.size(); i++)
    {
        std::string update_query = "UPDATE itementitytable SET count = ? WHERE dataId = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

        pstmt->setInt(1, itemCounts[i]);
        pstmt->setUInt64(2, ids[i]);
        pstmt->executeUpdate();
    }

    room->DoAsync(&DBManager::SetItemCount2, ids, player, itemCounts);
}

void DBManager::SetItemCount2(vector<uint64> ids, PlayerRef player, vector<uint32> itemCounts)
{
    
    Protocol::S_SETITEMCOUNT pkt;

    for(int i = 0; i < ids.size(); i++)
    {  
        player->GetInventory()->GetItem(ids[i])->SetCount(itemCounts[i], player);
        cout << ids[i] << "번 아이템 개수 : " << itemCounts[i] << endl;
        pkt.add_itemid(ids[i]);
        pkt.add_itemcount(itemCounts[i]);
    }
    
    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);
}

void DBManager::AddNewSkill(int32 skillId, PlayerRef player)
{
    // 1. DB에 스킬 추가
    std::string insert_query = "INSERT INTO characterskillinfotable (characterId, skillId, skillLevel, quickslot) VALUES (?, ?, ?, ?)";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    cout << "Add New Skill : " << player->GetDbId() << " (" << skillId << ")" << endl;
    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, skillId);
    pstmt->setInt(3, 1);
    pstmt->setInt(4, static_cast<int>(Protocol::QUICK_NONE));
    pstmt->executeUpdate();

    // 2. 메모리에 스킬 추가
    std::string update_query2 = "UPDATE characterinfotable SET skillpoint = ? WHERE id = ?";
    sql::PreparedStatement* pstmt2 = con->prepareStatement(update_query2);

    pstmt2->setInt(1, player->GetSkillComponent()->GetSkillPoint() - 1);
    pstmt2->setUInt64(2, player->GetDbId());

    pstmt2->executeUpdate();

    player->GetRoom()->DoAsync(&DBManager::UpdateSkillInMemory, skillId, 1, player);

    delete pstmt;
    delete pstmt2;
}

void DBManager::SetSkillLevel(int32 skillId, int32 skillLevel, PlayerRef player)
{
    std::string update_query = "UPDATE characterskillinfotable SET skillLevel = ? WHERE characterId = ? AND skillId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

    cout << player->GetDbId() << " Player's Skill (" << skillId << ") Level Up! ->" << skillLevel << endl;
    pstmt->setInt(1, skillLevel);
    pstmt->setUInt64(2, player->GetDbId());
    pstmt->setInt(3, skillId);
    pstmt->executeUpdate();

    std::string update_query2 = "UPDATE characterinfotable SET skillpoint = ? WHERE id = ?";
    sql::PreparedStatement* pstmt2 = con->prepareStatement(update_query2);

    pstmt2->setInt(1, player->GetSkillComponent()->GetSkillPoint() - 1);
    pstmt2->setUInt64(2, player->GetDbId());

    pstmt2->executeUpdate();


    player->GetRoom()->DoAsync(&DBManager::UpdateSkillInMemory, skillId, skillLevel, player);

    delete pstmt;
    delete pstmt2;
}

void DBManager::UpdateSkillInMemory(int32 skillId, int32 skillLevel, PlayerRef player)
{
    player->GetSkillComponent()->SetSkillLevel(skillId, skillLevel);
}


void DBManager::SetQuickSlot(Protocol::EQuickSlot slot, uint64 id, Protocol::EQuickElementType type, PlayerRef player)
{
    std::string update_query;
    if(type == Protocol::QUICK_ITEM)
    {
        update_query = "UPDATE itementitytable SET quickslot = ? WHERE dataId = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

        cout << "Item(" << id << ") has been registered at Slot " << slot << endl;
        pstmt->setInt(1, static_cast<int32>(slot));
        pstmt->setUInt64(2, id);
        pstmt->executeUpdate();
    }
    if(type == Protocol::QUICK_SKILL)
    {
        update_query = "UPDATE characterskillinfotable SET quickslot = ? WHERE characterId = ? AND skillId = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

        cout << "Skill(" << id << ") has been registered at Slot " << slot << endl;
        pstmt->setInt(1, static_cast<int32>(slot));
        pstmt->setUInt64(2, player->GetDbId());
        pstmt->setInt(3, static_cast<int32>(id));
        pstmt->executeUpdate();
    }


    player->GetRoom()->DoAsync(&DBManager::SetQuickSlot2, slot, id, type, player);
}

void DBManager::SetQuickSlot2(Protocol::EQuickSlot slot, uint64 id, Protocol::EQuickElementType type, PlayerRef player)
{
    if(slot == Protocol::QUICK_NONE)
        return;
        
    player->GetQuickSlotComponent()->SetElement(slot, type, id);
}

void DBManager::InitEquippedItem(uint64 dataId, PlayerRef player, Protocol::EItemType type, uint32 templateId)
{
    ItemRef item = ObjectUtils::CreateItem(type, dataId, templateId, 1);
}

void DBManager::EquipItem(uint64 dataId, PlayerRef player, RoomRef room, ItemRef unequipItem)
{
    // 1. 장착하려는 아이템 장착처리
    std::string update_query = "UPDATE itementitytable SET slotState = 1, slotIndex = -1 WHERE dataId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(update_query);

    pstmt->setUInt64(1, dataId);
    pstmt->executeUpdate();

    room->DoAsync(&DBManager::EquipItem2, dataId, player, unequipItem);
}

void DBManager::EquipItem2(uint64 dataId, PlayerRef player, ItemRef unequipItem)
{
    // 1. 장착된 아이템 메모리에서 처리
    
    int slotIndex = player->GetInventory()->GetItem(dataId)->GetSlotIndex();
    player->GetInventory()->EquipItem(dataId);

    Protocol::S_EQUIP pkt;
    pkt.set_equipdbid(dataId);
    pkt.set_previtemslotindex(-1);

    if (unequipItem != nullptr)
    {
        // 2. 장착 중인 아이템이 이미 있으면 장착 해제를 메모리에서 처리
        player->GetInventory()->AddItemToInventory(unequipItem, slotIndex, false, false);

        // 3. 장착 중인 아이템 해제를 DB에서 처리
        GDBManager->DoAsync(&DBManager::ItemSlotChange, unequipItem->GetDbId(), slotIndex);

        pkt.set_previtemdbid(unequipItem->GetDbId());
        pkt.set_previtemslotindex(slotIndex);

        cout << dataId << "번 아이템 장착, " << unequipItem->GetDbId() << "번 아이템은 " << slotIndex << "번 슬롯으로 이동" << endl;
    }

    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);
}


void DBManager::UnEquipItem(uint64 dataId, PlayerRef player, RoomRef room)
{
    cout << "UnEquip" << endl;
    ItemRef unequipItem = player->GetInventory()->GetItem(dataId);
    int slotIndex = player->GetInventory()->FindSlotIndex(unequipItem->GetItemType());
    
    // 1. 장착 해제를 메모리에서 처리
    player->GetInventory()->UnEquipItem(unequipItem);
    player->GetInventory()->AddItemToInventory(unequipItem, slotIndex, true, true);
   
    // 2. 장착 해제를 DB에서 처리
    GDBManager->DoAsync(&DBManager::ItemSlotChange, unequipItem->GetDbId(), slotIndex);
}

void DBManager::ItemSlotChange(uint64 itemDbId, int32 slotIndex)
{
    if(itemDbId < 0)
        return;

    std::string insert_query = "UPDATE itementitytable SET slotindex = ?, slotState = ? WHERE dataId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    pstmt->setInt(1, slotIndex);
    pstmt->setInt(2, (int)Protocol::SLOT_INVENTORY);
    pstmt->setUInt64(3, itemDbId);
   
    pstmt->executeUpdate();
}

void DBManager::LevelUp(PlayerRef player, int32 level)
{
    std::string insert_query = "UPDATE usercharactertable SET lvl = ? WHERE characterId = ?";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    pstmt->setInt(1, level);
    pstmt->setUInt64(2, player->GetDbId());

    pstmt->executeUpdate();

    std::string update_query2 = "UPDATE characterinfotable SET skillpoint = ? WHERE id = ?";
    sql::PreparedStatement* pstmt2 = con->prepareStatement(update_query2);


    pstmt2->setInt(1, player->GetSkillComponent()->GetSkillPoint());
    pstmt2->setUInt64(2, player->GetDbId());
    pstmt2->executeUpdate();

    LoadAvailableQuestsByLevel(player, level);
    delete pstmt;
    delete pstmt2;
}

void DBManager::LoadAvailableQuests(PlayerRef player)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM availablequesttable WHERE characterId = ?");
    pstmt->setUInt64(1, player->GetDbId());
    sql::ResultSet* res = pstmt->executeQuery();

    vector<int32> questIds;
    while (res->next())
    {
        int32 questId = res->getInt("questId");
        questIds.push_back(questId);
    }
    
    AddAvailableQuestsInMemory(player, questIds);
    // Clean up
    delete res;
    delete pstmt;
}

void DBManager::LoadInProgressQuests(PlayerRef player)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM inprogressquesttable WHERE characterId= ?");
    pstmt->setUInt64(1, player->GetDbId());
    sql::ResultSet* res = pstmt->executeQuery();

    vector<int32> questIds;
    while (res->next())
    {
        int32 questId = res->getInt("questId");
        questIds.push_back(questId);
        //sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM inprogressquesttable WHERE characterId= ?");
    }

    AddInProgressQuestsInMemory(player, questIds);
    
    // Clean up
    delete res;
    delete pstmt;
}

void DBManager::AddAvailableQuestsInMemory(PlayerRef player, const vector<int32>& ids)
{
    auto questIdsPtr = std::make_shared<vector<int32>>(ids);
    auto myLambda = [player, questIdsPtr]() {
            cout << player->GetDbId() << "번 플레이어의 시작 가능 퀘스트 (" << questIdsPtr->size() << ") 로딩" << endl;
            Protocol::S_QUEST pkt;
                for (int i = 0; i < questIdsPtr->size(); i++)
                {
                    pkt.add_questid((*questIdsPtr)[i]);
                    pkt.add_queststate(Protocol::QUEST_AVAILABLE);
                    player->GetQuestComponent()->AddToAvailable((*questIdsPtr)[i]);
                }
            SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
            player->session.lock()->Send(sendBuffer);
        };
    player->GetRoom()->DoAsync(myLambda);

}

void DBManager::AddInProgressQuestsInMemory(PlayerRef player, const vector<int32>& ids)
{
    auto questIdsPtr = std::make_shared<vector<int32>>(ids);
    auto myLambda = [player, questIdsPtr]() {
            Protocol::S_QUEST pkt;
            cout << player->GetDbId() << "번 플레이어의 진행 중인 퀘스트 (" << questIdsPtr->size() << ") 로딩" << endl;
            for (int i = 0; i < questIdsPtr->size(); i++)
            {
                cout << "Add InProgress Quest - " << (*questIdsPtr)[i] << endl;
                pkt.add_questid((*questIdsPtr)[i]);
                pkt.add_queststate(Protocol::QUEST_ACCEPT);
                player->GetQuestComponent()->AddToInProgress((*questIdsPtr)[i]);
            }
            SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
            player->session.lock()->Send(sendBuffer);
        };
    player->GetRoom()->DoAsync(myLambda);
    LoadQuestTargets(player, ids);
}

void DBManager::LoadQuestTargets(PlayerRef player, vector<int32> ids)
{
    shared_ptr<vector<tuple<int32, int32, int32>>> targetInfo = make_shared<vector<tuple<int32, int32, int32>>>();
    for (int i = 0; i < ids.size(); i++)
    {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM playerquesttargettable WHERE characterId= ? AND questId = ?");
        pstmt->setUInt64(1, player->GetDbId());
        pstmt->setInt(2, ids[i]);
        sql::ResultSet* res = pstmt->executeQuery();
        
        while (res->next())
        {
            int32 questId = res->getInt("questId");
            int32 targetIndex = res->getInt("targetIndex");
            int32 targetCount = res->getInt("targetCount");
   
            targetInfo->push_back(make_tuple(questId, targetIndex, targetCount));
        }

        // Clean up
        delete res;
        delete pstmt;
    }

    auto myLambda = [player, targetInfo]() {
    Protocol::S_LOAD_TARGET pkt;

    for (int i = 0; i < targetInfo->size(); i++)
    {
        pkt.add_questid(std::get<0>((*targetInfo)[i]));
        pkt.add_targetindex(std::get<1>((*targetInfo)[i]));
        pkt.add_targetcount(std::get<2>((*targetInfo)[i]));
        player->GetQuestComponent()->GetInProgressQuest(std::get<0>((*targetInfo)[i]))->SetTargetNumber(std::get<1>((*targetInfo)[i]), std::get<2>((*targetInfo)[i]));
    }
    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    player->session.lock()->Send(sendBuffer);

    cout << " Quest Target Info Send! " << endl;
    };
    player->GetRoom()->DoAsync(myLambda);
}

void DBManager::LoadAvailableQuestsByLevel(PlayerRef player, int32 level)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM questtable WHERE lvl = ? AND isLocked = 0");
    pstmt->setInt(1, level);

    sql::ResultSet* res = pstmt->executeQuery();

    vector<int32> questIds;
    while (res->next())
    {
        int32 questId = res->getInt("questId");
        sql::PreparedStatement* pstmt2 = con->prepareStatement("INSERT INTO availablequesttable (characterId, questId) VALUES (?, ?)");
        pstmt2->setUInt64(1, player->GetDbId());
        pstmt2->setInt(2, questId);

        pstmt2->executeQuery();
        delete pstmt2;
        questIds.push_back(questId);
    }

    AddAvailableQuestsInMemory(player, questIds);
    // Clean up
    delete res;
    delete pstmt;
}

void DBManager::LoadAvailableQuestsById(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM questtable WHERE questId = ?");
    pstmt->setInt(1, questId);
    sql::ResultSet* res = pstmt->executeQuery();

    vector<int32> ids;
    if (res->next())
    {
        int32 questId = res->getInt("questId");
        sql::PreparedStatement* pstmt2 = con->prepareStatement("INSERT INTO availablequesttable (characterId, questId) VALUES (?, ?)");
        pstmt2->setUInt64(1,player->GetDbId());
        pstmt2->setInt(2, questId);

        pstmt2->executeQuery();
        delete pstmt2;
        ids.push_back(questId);
    }
    AddAvailableQuestsInMemory(player, ids);

    // Clean up
    delete res;
    delete pstmt;
}

void DBManager::AcceptNewQuest(PlayerRef player, int32 questId)
{
    RemoveAvailableQuest(player, questId);
    AddInProgressQuest(player, questId);
    AddQuestTarget(player, questId);

    // questId번 퀘스트 수주 패킷 보내기!
    auto myLambda = [player, questId]() {
        Protocol::S_QUEST pkt;
        cout << player->GetDbId() << "번 플레이어 퀘스트 수주 (" << questId << ") 로딩" << endl;
        pkt.add_questid(questId);
        pkt.add_queststate(Protocol::QUEST_ACCEPT);
        player->GetQuestComponent()->AddToInProgress(questId, 0);

        SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
        player->session.lock()->Send(sendBuffer);
        };
    player->GetRoom()->DoAsync(myLambda);
}

void DBManager::AbandonQuest(PlayerRef player, int32 questId)
{
    RemoveInProgressQuest(player, questId);
    AddAvailableQuest(player, questId);

    // questId번 퀘스트 포기 패킷 보내기!
}

void DBManager::CompleteQuest(PlayerRef player, int32 questId)
{
    RemoveInProgressQuest(player, questId);

    sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO completedquesttable (characterId, questId, completedDate) VALUES (?, ?, NOW())");

    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, questId);

    // 쿼리 실행
    pstmt->executeUpdate();

    // Clean up
    delete pstmt;

    // questId번 퀘스트 완료 패킷 보내기!
    auto myLambda = [player, questId]() {
        Protocol::S_QUEST pkt;
        cout << player->GetDbId() << "번 플레이어 퀘스트 완료 (" << questId << ") 로딩" << endl;
        pkt.add_questid(questId);
        pkt.add_queststate(Protocol::QUEST_COMPLETED);

        SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
        player->session.lock()->Send(sendBuffer);

        };
    player->GetRoom()->DoAsync(myLambda);
    player->GetRoom()->DoAsync(&Room::GiveQuestRewards, player, questId);


    for (int i = 0; i < GDataManager->GetQuestData(questId)->linkedQuests.size(); i++)
    {
        LoadAvailableQuestsById(player, GDataManager->GetQuestData(questId)->linkedQuests[i]);
    }
}

void DBManager::AddQuestTarget(PlayerRef player, int32 questId)
{

    for(auto& targetInfo : GDataManager->GetQuestTargetData(questId))
    {
        // 나중에 target Count는 아이템의 경우 아이템 갯수를 센 다음에 초기화하기
        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO playerquesttargettable (characterId, questId, targetIndex, targetCount) VALUES (?, ?, ?, 0)");

        pstmt->setUInt64(1, player->GetDbId());
        pstmt->setInt(2, questId);
        pstmt->setInt(3, targetInfo.second->targetIndex);

        // 쿼리 실행
        pstmt->executeUpdate();
        delete pstmt;
    }


}

void DBManager::AddQuickQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO quickquesttable (characterId, questId, quickedDate) VALUES (?, ?, NOW())");

    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, questId);

    // 쿼리 실행
    pstmt->executeUpdate();
    delete pstmt;
}

void DBManager::RemoveQuickQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM quickquesttable WHERE characterId = ? AND questId = ?");

    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, questId);

    // 쿼리 실행
    pstmt->executeUpdate();
    delete pstmt;
}

void DBManager::LoadQuickQuestList(PlayerRef player)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM quickquesttable WHERE characterId = ? ORDER BY quickedDate ASC");
    pstmt->setUInt64(1, player->GetDbId());
    sql::ResultSet* res = pstmt->executeQuery();

    vector<int32> ids;
    while (res->next())
    {
        int32 questId = res->getInt("questId");
        ids.push_back(questId);
    }
    LoadQuickQuestList2(player, ids);

    // Clean up
    delete res;
    delete pstmt;
}

void DBManager::LoadQuickQuestList2(PlayerRef player, const vector<int32> quickIds)
{
    auto myLambda = [player, quickIds]() {
        Protocol::S_QUICK_QUEST pkt;
        for (int i = 0; i < quickIds.size(); i++)
        {
            pkt.add_questid(quickIds[i]);
        }

        SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
        player->session.lock()->Send(sendBuffer);
        };
    player->GetRoom()->DoAsync(myLambda);
}

void DBManager::SavePlayerInfo(PlayerRef player)
{
    // 1. hp, exp, 위치 정보 등 저장
    Protocol::ObjectInfo* objInfo = player->GetObjectInfo();
    shared_ptr<Protocol::StatInfo> statInfo = player->GetStatComponent()->GetStatInfo();

    sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE characterinfotable SET hp = ?, mp = ?, x = ?, y = ?, z = ?, exp = ?, skillpoint = ? WHERE id = ?");

    pstmt->setInt(1, statInfo->hp());
    pstmt->setInt(2, statInfo->mp());
    pstmt->setInt(3, objInfo->x());
    pstmt->setInt(4, objInfo->y());
    pstmt->setInt(5, objInfo->z());
    pstmt->setInt(6, statInfo->exp());
    pstmt->setInt(7, player->GetSkillComponent()->GetSkillPoint());
    pstmt->setUInt64(8, player->GetDbId());

    // 쿼리 실행
    pstmt->executeUpdate();
    cout << "Player " << player->GetDbId() << " -> PlayerInfo Save!" << endl;
    // 2. 퀘스트 진행도 저장

    unordered_map<int32, shared_ptr<Quest>> quests = player->GetQuestComponent()->GetInProgressQuests();
    
    pstmt = con->prepareStatement("UPDATE playerquesttargettable SET targetCount = ? WHERE characterId = ? AND questId = ? AND targetIndex = ?");

    for (const auto& quest : quests)
    {
        vector<shared_ptr<FQuestTarget>> targets = quest.second->GetAllTargetInfo();
        int32 questId = quest.second->GetQuestId();
        for (int i = 0; i < targets.size(); i++)
        {
       
            pstmt->setInt(1, targets[i]->targetNowNumber);
            pstmt->setUInt64(2, player->GetDbId());
            pstmt->setInt(3, questId);
            pstmt->setInt(4, targets[i]->targetIndex);
            pstmt->executeUpdate();
        }

    }
    delete pstmt;
    cout << "Player " << player->GetDbId() << " -> Quest Save!" << endl;
}



/*
        ==========  데이터를 가져와서 메모리에 올려놓는 함수들  ==========
*/
void DBManager::GetCharacterAttackData()
{
    if (!con)
        return;

    // 쿼리 실행
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM characterattacktable");

    // 결과 출력
    while (res->next()) {
        int id = res->getInt("classId");
        int sequence = res->getInt("sequence");
        float offsetX  = res->getDouble("offsetX");
        float offsetY = res->getDouble("offsetY");
        float offsetZ = res->getDouble("offsetZ");
        float x  = res->getDouble("x");
        float y  = res->getDouble("y");
        float z  = res->getDouble("z");
        float timeToHit  = res->getDouble("timetohit");
        float comboStart  = res->getDouble("combostart");
        float comboEnd  = res->getDouble("comboend");
        float timeToIdle  = res->getDouble("timetoidle");
        float damage  = res->getDouble("Damage");
        string colliderType  = res->getString("collidertype");

       shared_ptr<CharacterAttackInfo> newInfo = make_shared<CharacterAttackInfo>(sequence, offsetX, offsetY, offsetZ, x, y, z, timeToHit, comboStart, comboEnd, timeToIdle, damage, colliderType);
        GDataManager->characterAttackData[id][sequence] = newInfo;
    }
    delete res;
    delete stmt;
}

void DBManager::GetMonsterAttackData()
{
    if (!con)
        return;

    // 쿼리 실행
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM monsterattacktable");

    // 결과 출력
    while (res->next()) {
        int id = res->getInt("id");
        int sequence = res->getInt("sequence");
        float rangeOffset = res->getDouble("rangeOffset");
        float x = res->getDouble("x");
        float y = res->getDouble("y");
        float z = res->getDouble("z");
        float timeToHit = res->getDouble("timetohit");
        float timeToIdle = res->getDouble("timetoidle");
        string colliderType = res->getString("collidertype");

        shared_ptr<MonsterAttackInfo> newInfo = make_shared<MonsterAttackInfo>(sequence, rangeOffset, x, y, z, timeToHit, timeToIdle, colliderType);
        GDataManager->monsterAttackData[id][sequence] = newInfo;
    }

    // 메모리 해제
    delete res;
    delete stmt;
}

void DBManager::GetWarriorStatData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM warriorstat");

    while (res->next()) {
        int level = res->getInt("lvl");
        int maxHp = res->getInt("maxhp");
        int maxMp = res->getInt("maxmp");
        int attack = res->getInt("attack");
        int defense = res->getInt("defense");
        int maxExp = res->getInt("maxexp");

        shared_ptr<CharacterStatInfo> newInfo = make_shared<CharacterStatInfo>(level, maxHp, maxMp, attack, defense, maxExp);
        GDataManager->characterStat[Protocol::WARRIOR][level] = newInfo;

    }

    cout << "Warrior : " << GDataManager->characterStat[Protocol::WARRIOR][2]->hp << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetRogueStatData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM roguestat");

    while (res->next()) {
        int level = res->getInt("lvl");
        int maxHp = res->getInt("maxhp");
        int maxMp = res->getInt("maxmp");
        int attack = res->getInt("attack");
        int defense = res->getInt("defense");
        int maxExp = res->getInt("maxexp");

        shared_ptr<CharacterStatInfo> newInfo = make_shared<CharacterStatInfo>(level, maxHp, maxMp, attack, defense, maxExp);
        GDataManager->characterStat[Protocol::ROGUE][level] = newInfo;

    }
    cout << "Rogue : " << GDataManager->characterStat[Protocol::ROGUE][2]->hp << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetWizardStatData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM wizardstat");

    while (res->next()) {
        int level = res->getInt("lvl");
        int maxHp = res->getInt("maxhp");
        int maxMp = res->getInt("maxmp");
        int attack = res->getInt("attack");
        int defense = res->getInt("defense");
        int maxExp = res->getInt("maxexp");

        shared_ptr<CharacterStatInfo> newInfo = make_shared<CharacterStatInfo>(level, maxHp, maxMp, attack, defense, maxExp);
        GDataManager->characterStat[Protocol::WIZARD][level] = newInfo;

    }
    cout << "Wizard : " << GDataManager->characterStat[Protocol::WIZARD][2]->hp << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetWarriorSkillHitData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillhittable WHERE classId = 0");

    while (res->next()) {
        int id = res->getInt("skillId");
        int seq = res->getInt("sequence");
        float front = res->getDouble("offsetFront");
        float up = res->getDouble("offsetUp");
        float right = res->getDouble("offsetRight");
        float x = res->getDouble("rangeX");
        float y = res->getDouble("rangeY");
        float z = res->getDouble("rangeZ");
        float hitTime = res->getDouble("timeToHit");
        float idleTime = res->getDouble("timeToIdle");
        float cool = res->getDouble("coolDown");
        string hitType = res->getString("hitType");

        shared_ptr<SkillHitInfo> newInfo = make_shared<SkillHitInfo>(seq, front, up, right, x, y, z, hitTime, idleTime, cool, hitType);
        GDataManager->warriorSkillHitData[id][seq] = newInfo;

    }
    cout << "WARRIOR SKILL : " << GDataManager->warriorSkillHitData[1][1]->timeToIdle << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetRogueSkillHitData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillhittable WHERE classId = 1");

    while (res->next()) {
        int id = res->getInt("skillId");
        int seq = res->getInt("sequence");
        float front = res->getDouble("offsetFront");
        float up = res->getDouble("offsetUp");
        float right = res->getDouble("offsetRight");
        float x = res->getDouble("rangeX");
        float y = res->getDouble("rangeY");
        float z = res->getDouble("rangeZ");
        float hitTime = res->getDouble("timeToHit");
        float idleTime = res->getDouble("timeToIdle");
        float cool = res->getDouble("coolDown");
        string hitType = res->getString("hitType");

        shared_ptr<SkillHitInfo> newInfo = make_shared<SkillHitInfo>(seq, front, up, right, x, y, z, hitTime, idleTime, cool, hitType);
        GDataManager->rogueSkillHitData[id][seq] = newInfo;

    }
    cout << "Rogue SKILL : " << GDataManager->warriorSkillHitData[0][1]->offsetFront << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetWizardSkillHitData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillhittable WHERE classId = 2");

    while (res->next()) {
        int id = res->getInt("skillId");
        int seq = res->getInt("sequence");
        float front = res->getDouble("offsetFront");
        float up = res->getDouble("offsetUp");
        float right = res->getDouble("offsetRight");
        float x = res->getDouble("rangeX");
        float y = res->getDouble("rangeY");
        float z = res->getDouble("rangeZ");
        float hitTime = res->getDouble("timeToHit");
        float idleTime = res->getDouble("timeToIdle");
        float cool = res->getDouble("coolDown");
        string hitType = res->getString("hitType");

        shared_ptr<SkillHitInfo> newInfo = make_shared<SkillHitInfo>(seq, front, up, right, x, y, z, hitTime, idleTime, cool, hitType);
        GDataManager->wizardSkillHitData[id][seq] = newInfo;

    }
    cout << "Rogue SKILL : " << GDataManager->wizardSkillHitData[0][1]->offsetFront << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetWarriorSkillDamageData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skilldamagetable WHERE classId = 0");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");
      

        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillDamageData[classId][skillId] = newInfo;
    }
    cout << "Rogue SKILL : " << GDataManager->wizardSkillHitData[0][1]->offsetFront << std::endl;
    delete res;
    delete stmt;
}

void DBManager::GetRogueSkillDamageData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skilldamagetable WHERE classId = 1");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");


        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillDamageData[classId][skillId] = newInfo;
    }

    delete res;
    delete stmt;
}

void DBManager::GetWizardSkillDamageData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skilldamagetable WHERE classId = 2");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");


        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillDamageData[classId][skillId] = newInfo;
    }
    delete res;
    delete stmt;
}

void DBManager::GetWarriorSkillMpData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillmptable WHERE classId = 0");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");


        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillMpData[classId][skillId] = newInfo;
    }
    delete res;
    delete stmt;
}

void DBManager::GetRogueSkillMpData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillmptable WHERE classId = 1");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");


        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillMpData[classId][skillId] = newInfo;
    }
    delete res;
    delete stmt;
}

void DBManager::GetWizardSkillMpData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM skillmptable WHERE classId = 2");

    while (res->next()) {
        int classId = res->getInt("classId");
        int skillId = res->getInt("skillId");
        array<int, 11> arr;
        arr[1] = res->getInt("lvl1");
        arr[2] = res->getInt("lvl2");
        arr[3] = res->getInt("lvl3");
        arr[4] = res->getInt("lvl4");
        arr[5] = res->getInt("lvl5");
        arr[6] = res->getInt("lvl6");
        arr[7] = res->getInt("lvl7");
        arr[8] = res->getInt("lvl8");
        arr[9] = res->getInt("lvl9");
        arr[10] = res->getInt("lvl10");
        int maxLvl = res->getInt("maxLevel");


        shared_ptr<SkillDataByLevel> newInfo = make_shared<SkillDataByLevel>(classId, skillId, arr);
        GDataManager->skillMpData[classId][skillId] = newInfo;
    }
    delete res;
    delete stmt;
}

void DBManager::GetMonsterStatData()
{
    if (!con)
        return;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM monsterstat");

    while (res->next()) {
        int id = res->getInt("monsterId");
        int level = res->getInt("lvl");
        string name = res->getString("mobName");
        int maxHp = res->getInt("maxHp");
        int attack = res->getInt("attack");
        int defense = res->getInt("defense");
        int exp = res->getInt("exp");

        shared_ptr<MonsterStatInfo> newInfo = make_shared<MonsterStatInfo>(id, level, name, maxHp, attack, defense, exp);
        GDataManager->monsterStat[id] = newInfo;

    }

    delete res;
    delete stmt;
}

void DBManager::GetWeaponData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM weapontable");

    while (res->next()) {
        uint32 id = res->getUInt("templateId");
        Protocol::EWeaponData weaponData = (Protocol::EWeaponData)res->getInt("weaponType");
        Protocol::EItemGrade grade = (Protocol::EItemGrade)res->getInt("grade");
        uint32 usableLevel = res->getUInt("usableLevel");
        string name = res->getString("itemName");
        string desc = res->getString("itemDesc");
        uint32 power = res->getUInt("power");
        uint32 buy = res->getUInt("buyPrice");
        uint32 sell = res->getUInt("sellPrice");
        Protocol::ECharacterType type = (Protocol::ECharacterType)res->getInt("class");

        shared_ptr<WeaponInfo> weapon = make_shared<WeaponInfo>(id, name, desc, grade, power, buy, sell, usableLevel, weaponData, type);

        GDataManager->weaponData[id] = weapon;
    }

    delete res;
    delete stmt;
}

void DBManager::GetArmorData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM armortable");

    while (res->next()) {
        uint32 id = res->getUInt("templateId");
        Protocol::EArmorData armorData = (Protocol::EArmorData)res->getInt("armorType");
        Protocol::EItemGrade grade = (Protocol::EItemGrade)res->getInt("grade");
        uint32 usableLevel = res->getUInt("usableLevel");
        string name = res->getString("itemName");
        string desc = res->getString("itemDesc");
        uint32 power = res->getUInt("power");
        uint32 buy = res->getUInt("buyPrice");
        uint32 sell = res->getUInt("sellPrice");
        Protocol::ECharacterType type = (Protocol::ECharacterType)res->getInt("class");

        shared_ptr<ArmorInfo> armor = make_shared<ArmorInfo>(id, name, desc, grade, power, buy, sell, usableLevel, armorData, type);

        GDataManager->armorData[id] = armor;
    }

    cout << "Armor : " << GDataManager->armorData[20]->itemDesc << endl;
    delete res;
    delete stmt;
}

void DBManager::GetConsumableData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM consumabletable");

    while (res->next()) {
        uint32 id = res->getUInt("templateId");
        Protocol::EHealType healData = (Protocol::EHealType)res->getInt("healType");
        Protocol::EItemGrade grade = (Protocol::EItemGrade)res->getInt("grade");
        string name = res->getString("itemName");
        string desc = res->getString("itemDesc");
        uint32 power = res->getUInt("power");
        uint32 buy = res->getUInt("buyPrice");
        uint32 sell = res->getUInt("sellPrice");

        shared_ptr<ConsumableInfo> consumable = make_shared<ConsumableInfo>(id, name, desc, grade, power, buy, sell, healData);

        GDataManager->consumableData[id] = consumable;
    }

    //cout << "Armor : " << GDataManager->armorData[20]->itemDesc << endl;
    delete res;
    delete stmt;
}

void DBManager::GetQuestInfoData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM questtable");

    while (res->next()) {
        int32 id = res->getInt("questId");
        Protocol::EQuestGrade grade = static_cast<Protocol::EQuestGrade>(res->getInt("questGrade"));
        int32 startNpc = res->getInt("startNpc");
        int32 completeNpc = res->getInt("completeNpc");
        int32 level = res->getInt("lvl");
        string linkedQuest = res->getString("linkedQuests");
        int32 exp = res->getInt("exp");
        int32 gold = res->getInt("gold");

        vector<int32> linkedQuests;
        if (linkedQuest != "None")
        {
            stringstream sstream(linkedQuest);
            string stringId;
            while (sstream >> stringId)
            {
                int qId = stoi(stringId);
                linkedQuests.push_back(qId);
            }
        }
        
        
        shared_ptr<FQuestInfo> questInfo = make_shared<FQuestInfo>(id, grade, startNpc, completeNpc, level, linkedQuests, exp, gold);

        GDataManager->questData[id] = questInfo;
        cout << "Quest Init : " << id << endl;
    }

   
    delete res;
    delete stmt;
}

void DBManager::GetQuestTargetData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM questtargettable");

    while (res->next()) {
        int32 id = res->getInt("questId");
        int32 index = res->getInt("targetIndex");
        Protocol::ETargetType type = static_cast<Protocol::ETargetType>(res->getInt("targetType"));
        int32 targetId = res->getInt("targetId");
        int32 targetNumber = res->getInt("targetNumber");


        shared_ptr<FQuestTargetInfo> targetInfo = make_shared<FQuestTargetInfo>(id, index, type, targetId, targetNumber);

        GDataManager->questTargetData[id][index] = targetInfo;
    }


    delete res;
    delete stmt;
}

void DBManager::GetQuestRewardData()
{
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM questrewardtable");

    while (res->next()) {
        int32 questId = res->getInt("questId");
        Protocol::EItemType type = static_cast<Protocol::EItemType>(res->getInt("itemType"));
        int32 itemId = res->getInt("itemId");
        int32 itemCount = res->getInt("itemCount");


        shared_ptr<FQuestRewardInfo> rewardInfo = make_shared<FQuestRewardInfo>(questId, type, itemId, itemCount);

       
        GDataManager->questRewardData[questId].push_back(rewardInfo);
    }


    delete res;
    delete stmt;
}

vector<SpawnerInfo> DBManager::GetSpawnerData()
{
    //if (!con)
    //    return vector;

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM spawnerdata");

    vector<SpawnerInfo> v;
    while (res->next()) {
        int id = res->getInt("id");
        int roomid = res->getInt("roomid");
        float x = res->getDouble("x");
        float y = res->getDouble("y");
        float z = res->getDouble("z");
        string monsters = res->getString("monsters");

        SpawnerInfo info(id, roomid, x, y, z, monsters);
        v.push_back(info);
    }

    delete res;
    delete stmt;

    cout << "Get Size : " << v.size() << endl;
    return v;
}

vector<UserBaseInfo> DBManager::GetCharacterBaseInfoList(uint64 userId)
{
    vector<UserBaseInfo> v;
    // Create a prepared statement
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM usercharactertable WHERE userid = ?");

    // Bind the value to the placeholder
    pstmt->setUInt64(1, userId); // 1-based index

    // Execute the query
    sql::ResultSet* res = pstmt->executeQuery();

    int64 dbId;
    // Process the result set
    while (res->next())
    {
        cout << "Find Character : " << userId << endl;
        // while문 돌면서 UserBaseInfo 생성하고
        uint64 id = res->getUInt64("userId");
        uint64 characterId = res->getUInt64("characterId");
        string characterName = res->getString("characterName");
        int level = res->getInt("lvl");
        Protocol::ECharacterType type = (Protocol::ECharacterType)res->getInt("class");

        v.push_back(UserBaseInfo(id, characterId, characterName, level, type));
    }
    
    return v;
}

UserBaseInfo DBManager::GetUserBaseInfo(uint64 characterId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM usercharactertable WHERE characterId = ?");

    // Bind the value to the placeholder
    pstmt->setUInt64(1, characterId); // 1-based index

    // Execute the query
    sql::ResultSet* res = pstmt->executeQuery();

    int64 dbId;
    // Process the result set
    if (res->next())
    {
        // while문 돌면서 UserBaseInfo 생성하고
        uint64 id = res->getUInt64("userId");
        uint64 characterId = res->getUInt64("characterId");
        string characterName = res->getString("characterName");
        int level = res->getInt("lvl");
        Protocol::ECharacterType type = (Protocol::ECharacterType)res->getInt("class");

        return UserBaseInfo(id, characterId, characterName, level, type);
    }
   
    return UserBaseInfo();
}

CharacterBaseInfo DBManager::GetCharacterBaseInfo(uint64 characterId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM characterinfotable WHERE id = ?");

    // Bind the value to the placeholder
    pstmt->setUInt64(1, characterId); // 1-based index

    // Execute the query
    sql::ResultSet* res = pstmt->executeQuery();

    // Process the result set
    if (res->next())
    {
        // while문 돌면서 UserBaseInfo 생성하고
        uint64 id = res->getUInt64("id");
        int32 hp = res->getInt("hp");
        int32 mp = res->getInt("mp");
        int32 exp = res->getInt("exp");
        float x = res->getDouble("x");
        float y = res->getDouble("y");
        float z = res->getDouble("z");
        int32 skillPoint = res->getDouble("skillpoint");
        int32 gold = res->getDouble("gold");
        

        return CharacterBaseInfo(id, hp, mp, x, y, z, exp, skillPoint, gold);
    }

    return CharacterBaseInfo();
}

bool DBManager::CreateNewCharacter(uint64 userId, string name, Protocol::ECharacterType type, uint64& characterId)
{
    std::string insert_query = "INSERT INTO usercharactertable (userId, characterName, lvl, class) VALUES (?, ?, ?, ?)";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    cout << "Create New Character : " << name << endl;
    pstmt->setUInt64(1, userId);
    pstmt->setString(2, name);
    pstmt->setInt(3, 1);
    pstmt->setInt(4, (int)type);
 
    pstmt->executeUpdate();

    sql::PreparedStatement* id_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
    sql::ResultSet* res = id_stmt->executeQuery();
    
    if (res->next()) {
        characterId = res->getUInt64(1); // 마지막으로 삽입된 ID 가져오기
        delete res;
        delete id_stmt;
        delete pstmt;
        return true;
    }
    
    delete res;
    delete id_stmt;
    delete pstmt;

    return true;
}

void DBManager::SetNewCharacterInfo(uint64 characterId, Protocol::ECharacterType type)
{
    std::string insert_query = "INSERT INTO characterinfotable (id, hp, mp, x, y, z, exp, skillpoint, gold) VALUES (?, ?, ?, ?, ?, ?, ?, 0, 10000)";
    sql::PreparedStatement* pstmt = con->prepareStatement(insert_query);

    auto stat = GDataManager->GetCharacterStat(type, 1);

    pstmt->setUInt64(1, characterId);
    pstmt->setInt(2, stat->hp);
    pstmt->setInt(3, stat->mp);
    pstmt->setInt(4, -4020);
    pstmt->setInt(5, -2360);
    pstmt->setInt(6, 100);
    pstmt->setInt(7, 0);

    // 2. 추가한 아이템 데이터를 이용해 GameRoom의 JobQueue에 AddItem2 추가하기.
    pstmt->executeUpdate();

    delete pstmt;
}

void DBManager::AddAvailableQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO availablequesttable (characterId, questId) VALUES (?, ?)");

    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, questId);

    // 쿼리 실행
    pstmt->executeUpdate();

    // Clean up
    delete pstmt;
    auto myLambda = [player, questId]() {
        cout << player->GetDbId() << "번 플레이어 " << questId << "번 퀘스트 시작 가능" << endl;
        player->GetQuestComponent()->AddToAvailable(questId);
        };

    player->GetRoom()->DoAsync(myLambda);
}

void DBManager::AddInProgressQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO inprogressquesttable (characterId, questId, targetCount, receivedDate) VALUES (?, ?, 0, NOW())");

    pstmt->setUInt64(1, player->GetDbId());
    pstmt->setInt(2, questId);

    // 쿼리 실행
    pstmt->executeUpdate();

    // Clean up
    delete pstmt;

    auto myLambda = [player, questId]() {
        cout << player->GetDbId() << "번 플레이어 " << questId << "번 퀘스트 수주" << endl;
        player->GetQuestComponent()->AddToAvailable(questId);
        };

    player->GetRoom()->DoAsync(myLambda);
}
void DBManager::RemoveAvailableQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM availablequesttable WHERE characterId = ? AND questId = ?");
    pstmt->setUInt64(1, player->GetDbId()); // id 값 세팅
    pstmt->setInt(2, questId); // id 값 세팅

    // 쿼리 실행
    pstmt->executeUpdate();

    // Clean up
    delete pstmt;

    auto myLambda = [player, questId]() {
        player->GetQuestComponent()->DeleteFromAvailable(questId);
        };

    player->GetRoom()->DoAsync(myLambda);
}

void DBManager::RemoveInProgressQuest(PlayerRef player, int32 questId)
{
    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM inprogressquesttable WHERE characterId = ? AND questId = ?");
    pstmt->setUInt64(1, player->GetDbId()); // id 값 세팅
    pstmt->setInt(2, questId); // id 값 세팅

    // 쿼리 실행
    pstmt->executeUpdate();

    // Clean up
    delete pstmt;

    auto myLambda = [player, questId]() {
        player->GetQuestComponent()->DeleteFromInProgress(questId);
        };

    player->GetRoom()->DoAsync(myLambda);
}
