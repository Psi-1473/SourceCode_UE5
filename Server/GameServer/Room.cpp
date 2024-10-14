#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "Inventory.h"
#include "GameSession.h"
#include "Collider.h"
#include "ColliderUtils.h"
#include "MatrixHelper.h"
#include "ObjectUtils.h"
#include "Monster.h"
#include "MonsterSpawner.h"
#include "DBStruct.h"
#include "DBManager.h"
#include "BattleManager.h"
#include "DataManager.h"
#include "PlayerStatComponent.h"
#include "SkillComponent.h"
#include "AttackComponent.h"
#include "QuickSlotComponent.h"
#include "QuestComponent.h"
#include "DBStruct.h"

#include <sstream>

RoomRef GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

void Room::SetEnterPlayerInfo(PlayerRef player, uint64 characterId)
{
	GDBManager->DoAsync(&DBManager::EnterPlayer, player, characterId, PTR_CAST(Room, THIS));

}

void Room::SetPlayerComponentInfo(PlayerRef player)
{
	// 1. 아이템 정보 업데이트
	GDBManager->DoAsync(&DBManager::SetItems, PTR_CAST(Room, THIS), player);

	// 2. 스킬 정보 업데이트
	GDBManager->DoAsync(&DBManager::SetSkills, PTR_CAST(Room, THIS), player);

	// 아이템과 스킬 정보가 업데이트 되면서 그 내부적으로 자동으로 퀵슬롯 정보 업데이트

	// 3. 퀘스트 정보 업데이트
	GDBManager->DoAsync(&DBManager::LoadAvailableQuests, player);
	GDBManager->DoAsync(&DBManager::LoadInProgressQuests, player);
	


	// 4. 퀵슬롯에 등록된 퀘스트 정보 업데이트
	GDBManager->DoAsync(&DBManager::LoadQuickQuestList, player);

}

bool Room::HandleEnterPlayer(PlayerRef player, const UserBaseInfo base1, const CharacterBaseInfo base2)
{
	bool success = EnterPlayer(player);

	//랜덤 위치
	player->SetPlayerInfo(base1, base2);

	//player->GetObjectInfo()->set_objtype(Protocol::ROGUE);
	
	// 입장 사실을 신입 플레이어에게 알린다
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);
		enterGamePkt.set_skillpoint(player->GetSkillComponent()->GetSkillPoint());
		enterGamePkt.set_playername(base1.characterName);
		cout << "Player Name : " << base1.characterName << endl;
	
		Protocol::ObjectInfo* objectInfo = new Protocol::ObjectInfo();
		Protocol::StatInfo* statInfo = new Protocol::StatInfo();

		objectInfo->CopyFrom(*player->GetObjectInfo());
		statInfo->CopyFrom(*player->GetStatComponent()->GetStatInfo());

		enterGamePkt.set_allocated_player(objectInfo);
		enterGamePkt.set_allocated_stat(statInfo);
	
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}
	
	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;
	
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		Protocol::StatInfo* statInfo = spawnPkt.add_stats();

		objectInfo->CopyFrom(*player->GetObjectInfo());
		statInfo->CopyFrom(*player->GetStatComponent()->GetStatInfo());
		spawnPkt.add_playername(player->GetPlayerName());
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->GetObjectInfo()->object_id());
	}
	
	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
	{
		Protocol::S_SPAWN spawnPkt;
	
		for (auto& info : _players)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			Protocol::StatInfo* statInfo = spawnPkt.add_stats();
			objectInfo->CopyFrom(*info.second->GetObjectInfo());
			statInfo->CopyFrom(*info.second->GetStatComponent()->GetStatInfo());
			spawnPkt.add_playername(PTR_CAST(Player, info.second)->GetPlayerName());
		}
	
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 몬스터 목록을 신입 플레이어한테 전송
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& info : _monsters)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			Protocol::StatInfo* statInfo = spawnPkt.add_stats();
			spawnPkt.add_playername(to_string(PTR_CAST(Monster, info.second)->GetTemplateId()));
			objectInfo->CopyFrom(*info.second->GetObjectInfo());
			statInfo->CopyFrom(*info.second->GetStatComponent()->GetStatInfo());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장한 플레이어의 스탯 정보를 전송
	{
		player->SendStatPacket();
		player->GetInventory()->SetGold(base2.gold);
	}
	//DoTimer(3000, &Room::SpawnMonster);
	return success;
}


bool Room::HandleLeavePlayer(PlayerRef player)
{
	if (player == nullptr)
		return false;


	const uint64 objectId = player->GetObjectInfo()->object_id();
	bool success = LeavePlayer(objectId);
	cout << "Leave Game" << endl;
	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	//GSessiomManager.Remove(
	return success;
}

void Room::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if(_players.find(objectId) == _players.end())
		return;
	
	// 클라가 보내준 정보 서버에도 적용시키고 가지고 있기
	CreatureRef& player = _players[objectId];
	player->GetObjectInfo()->CopyFrom(pkt.info());
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::ObjectInfo* info = movePkt.mutable_info();
			Protocol::PlayerMoveInfo* moveInfo = movePkt.mutable_playerinfo();

			info->CopyFrom(pkt.info());
			moveInfo->CopyFrom(pkt.playerinfo());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
		
	}
}

void Room::HandleAttack(Protocol::C_ATTACK pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_players.find(objectId) == _players.end())
		return;
	
	CreatureRef& character = _players[objectId];
	character->GetObjectInfo()->CopyFrom(pkt.info());

	PlayerRef player = PTR_CAST(Player, character);

	player->Attack(pkt);
}

void Room::HandleUseSkill(PlayerRef player, Protocol::C_USESKILL pkt)
{
	player->UseSkill(pkt);
}

void Room::HandleDamaged(CreatureRef creature, uint64 attackerId, float damage)
{
	Protocol::S_DAMAGED pkt;

	Protocol::ObjectInfo* objInfo = pkt.mutable_info();
	objInfo->CopyFrom(*creature->GetObjectInfo());

	if(PTR_CAST(Monster, creature) != nullptr)
	{
		pkt.set_damage(damage);
		pkt.set_attackerid(attackerId);
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer);
}

void Room::HandleDie(CreatureRef creature)
{
	Protocol::S_DIE pkt;
	Protocol::ObjectInfo* info = pkt.mutable_info();
	info->CopyFrom(*creature->GetObjectInfo());
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer);

}

void Room::HandleAddItem(Protocol::C_ADDITEM pkt, PlayerRef player)
{
	uint32 templateId = pkt.templateid();
	AddItem(pkt.itemtype(), pkt.templateid(), pkt.count(), player);

	if (pkt.gettype() == Protocol::GET_BUY)
	{
		// 골드..
		int32 price = 0;
		if(pkt.itemtype() == Protocol::ITEM_ARMOR)
			price = GDataManager->GetArmorData(pkt.templateid())->buyPrice;
		if (pkt.itemtype() == Protocol::ITEM_WEAPON)
			price = GDataManager->GetWeaponData(pkt.templateid())->buyPrice;
		if (pkt.itemtype() == Protocol::ITEM_CONSUMABLE)
			price = GDataManager->GetConsumableData(pkt.templateid())->buyPrice;

		player->GetInventory()->AddGold(price * -1);
	}
	
}

void Room::HandleSetGold(Protocol::C_SETGOLD pkt, PlayerRef player)
{
	player->GetInventory()->AddGold(pkt.gold());
}

void Room::AddItem(Protocol::EItemType itemType, int32 itemId, int32 count, PlayerRef player)
{
	std::cout << "Add Item(" << itemId << ")" << " ---- Count = " << count << std::endl;
	
	uint64 dbId;
	bool stackable = player->GetInventory()->GetStackableDbId(itemType, itemId, dbId, count);
	
	if (stackable)
	{
		uint32 stackCount = player->GetInventory()->GetItem(dbId)->GetCount() + count; // 1은 잠시 하드코딩, 나중에 수정
		GDBManager->DoAsync(&DBManager::SetItemCountSingle, dbId, player, PTR_CAST(Room, THIS), stackCount);
	}
	else
	{
		GDBManager->DoAsync(&DBManager::AddItem1, itemType, (uint32)itemId, Protocol::QUICK_NONE, player, PTR_CAST(Room, THIS), count);
	}


}

void Room::HandleSwapItem(Protocol::C_ITEMSWAP pkt)
{
	const uint64 objectId = pkt.playerid();
	if (_players.find(objectId) == _players.end())
		return;

	CreatureRef& character = _players[objectId];
	PlayerRef player = PTR_CAST(Player, character);


	// 1. 메모리 상에서 정보 업데이트
	player->GetInventory()->SwapItem(pkt);

	// 2. DB에서 정보 업데이트
	GDBManager->DoAsync(&DBManager::ItemSlotChange, pkt.item1_dbid(), pkt.item1_slot());
	GDBManager->DoAsync(&DBManager::ItemSlotChange, pkt.item2_dbid(), pkt.item2_slot());
}

void Room::HandleQuickSlot(Protocol::C_SET_QUICK pkt, PlayerRef player)
{
	GDBManager->DoAsync(&DBManager::SetQuickSlot, pkt.slot(), pkt.elementid(), pkt.type(), player);
}

void Room::HandleEquipItem(Protocol::C_EQUIP pkt, PlayerRef player)
{
	shared_ptr<Item> item = player->GetInventory()->GetItem(pkt.itemid());
	int equipSlotIndex = (item->GetItemType() == Protocol::ITEM_WEAPON) ? 5 : (int)static_pointer_cast<ArmorInfo>(item->GetItemInfo())->armorType;

	shared_ptr<Item> unequipItem = player->GetInventory()->GetEquipItem(equipSlotIndex);

	GDBManager->DoAsync(&DBManager::EquipItem, pkt.itemid(), player, PTR_CAST(Room, THIS), unequipItem);
}

void Room::HandleUnEquipItem(Protocol::C_UNEQUIP pkt, PlayerRef player)
{
	shared_ptr<Item> item = player->GetInventory()->GetItem(pkt.itemid());
	int equipSlotIndex = (item->GetItemType() == Protocol::ITEM_WEAPON) ? 5 : (int)static_pointer_cast<ArmorInfo>(item->GetItemInfo())->armorType;

	shared_ptr<Item> unequipItem = player->GetInventory()->GetEquipItem(equipSlotIndex);

	DoAsync(&DBManager::UnEquipItem, pkt.itemid(), player, PTR_CAST(Room, THIS));
}

void Room::HandleUseItem(Protocol::C_USEITEM pkt, PlayerRef player)
{
	ItemRef item = player->GetInventory()->GetItem(pkt.itemid());

	// DB에서 아이템 수 줄이고 효과 적용
	GDBManager->DoAsync(&DBManager::RemoveOrDecrementSingle, pkt.itemid(), (uint32)1, player, PTR_CAST(Room, THIS));
	item->UseItem(player);
	
}

void Room::HandleRemoveItem(Protocol::C_REMOVEITEM pkt, PlayerRef player)
{
	vector<uint64> ids;
	vector<uint32> counts;
	
	for (int i = 0; i < pkt.itemid_size(); i++)
	{
		ids.push_back(pkt.itemid().Get(i));
		counts.push_back(pkt.removecount().Get(i));
	}
	GDBManager->DoAsync(&DBManager::RemoveOrDecrement, ids, counts, player, PTR_CAST(Room, THIS));

}

void Room::HandleUpgradeSkill(Protocol::C_UPGRADESKILL pkt, PlayerRef player)
{
	int skillLevel = player->GetSkillComponent()->GetSkillLevel(pkt.skillid());
	
	if (skillLevel == 0)
	{
		// 새로 스킬 정보 DB에 추가
		GDBManager->DoAsync(&DBManager::AddNewSkill, pkt.skillid(), player);
	}
	else
	{
		// 스킬 업그레이드
		GDBManager->DoAsync(&DBManager::SetSkillLevel, pkt.skillid(), skillLevel + 1, player);
	}
}

void Room::HandleQuest(Protocol::C_QUEST pkt, PlayerRef player)
{
	// 타입에 따라 처리
	for(int i = 0; i < pkt.questid_size(); i++)
	{
		switch (pkt.queststate().at(i))
		{
		case Protocol::QUEST_AVAILABLE:
			break;
		case Protocol::QUEST_ACCEPT:
			GDBManager->DoAsync(&DBManager::AcceptNewQuest, player, pkt.questid().at(i));
			break;
		case Protocol::QUEST_COMPLETED:
			GDBManager->DoAsync(&DBManager::CompleteQuest, player, pkt.questid().at(i));
			break;
		case Protocol::QUEST_ABANDON:
			GDBManager->DoAsync(&DBManager::AbandonQuest, player, pkt.questid().at(i));
			break;
		default:
			break;
		}
	}
}

void Room::HandleRenewTarget(Protocol::C_RENEW_TARGET pkt, PlayerRef player)
{
	if (pkt.targettype() == Protocol::TARGET_NPC)
	{
		player->GetQuestComponent()->TargetUpSpecificQuest(pkt.questid(), pkt.targetid(), pkt.targettype(), pkt.targetcount());
	}
	else
	{
		player->GetQuestComponent()->TargetUp(pkt.targetid(), pkt.targettype(), pkt.targetcount());
	}
}

void Room::HandleQuickQuest(Protocol::C_QUICK_QUEST pkt, PlayerRef player)
{
	if (pkt.isremove())
	{
		GDBManager->DoAsync(&DBManager::RemoveQuickQuest, player, pkt.questid());
	}
	else
	{
		GDBManager->DoAsync(&DBManager::AddQuickQuest, player, pkt.questid());
	}
}


bool Room::SpawnMonster(MonsterRef monster, bool isNew)
{
	Protocol::S_SPAWN pkt;
	Protocol::ObjectInfo* info = pkt.add_players();
	Protocol::StatInfo* stat = pkt.add_stats();
	info->CopyFrom(*monster->GetObjectInfo());
	stat->CopyFrom(*monster->GetStatComponent()->GetStatInfo());
	pkt.add_playername(to_string(monster->GetTemplateId()));
	
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer);

	if(isNew)
	{
		_monsters.insert(make_pair(monster->GetObjectInfo()->object_id(), monster));
		monster->room.store(GetRoomRef());
		DoAsync(&Room::UpdateTick, static_pointer_cast<BaseObject>(monster));
	}
	
	return true;
}

void Room::UpdateTick(shared_ptr<class BaseObject> object)
{
	object->Update();
}

RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::EnterPlayer(PlayerRef player)
{
	// 있다면 문제가 있다.
	if (_players.find(player->GetObjectInfo()->object_id()) != _players.end())
		return false;

	_players.insert(make_pair(player->GetObjectInfo()->object_id(), player));
	player->room.store(GetRoomRef());
	
	return true;
}


bool Room::LeavePlayer(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_players.find(objectId) == _players.end())
		return false;
	
	CreatureRef player = _players[objectId];
	player->room.store(weak_ptr<Room>());

	_players.erase(objectId);

	return true;
}

void Room::Respawn(uint64 spawnerId)
{
	_spawners[spawnerId]->Respawn();
}

void Room::HitCheck(AttackParam params)
{
	auto attackerState = params.attacker->GetObjectInfo()->state();
	if(attackerState != Protocol::STATE_ATTACK && attackerState != Protocol::STATE_SKILL)
		return;

	unordered_map<uint64, CreatureRef> objects = (params.attacker->GetObjectInfo()->objtype() == Protocol::MONSTER) ? _players : _monsters;

	GBattleManager->AttackHitCheck(params, objects);
}

void Room::SetCreatureAttackable(CreatureRef object, bool value, int attackSequence)
{
	if(object == nullptr)
		return;

	if (attackSequence != object->GetSAttackComponent()->GetAttackSequence())
		return;
	object->GetSAttackComponent()->SetCanAttack(value);
	if(PTR_CAST(Player, object) != nullptr)
	{
		Protocol::S_SET_DATA pkt;
		pkt.set_object_id(object->GetObjectInfo()->object_id());
		if(value == true)
		{
			//std::cout << "Start Combo Time" << std::endl;
			pkt.set_datatype(Protocol::EPlayerData::DATA_ATTACKABLE);
		}
		else
		{
			//std::cout << "End Combo Time" << std::endl;
			pkt.set_datatype(Protocol::EPlayerData::DATA_UNATTACKABLE);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Broadcast(sendBuffer);
	}
}

void Room::SetCreatureIdle(CreatureRef object, Protocol::EState prevState)
{
	//std::cout << "Player Set Idle" << std::endl;
	if(prevState != object->GetObjectInfo()->state())
		return;
	// 1. attack 관련 데이터 싹 리셋
	object->GetSAttackComponent()->SetCanAttack(true);
	object->GetSAttackComponent()->SetAttackSequence(0);

	// 2. 플레이어 Idle로 설정하고 패킷 보내기
	object->GetObjectInfo()->set_state(Protocol::STATE_IDLE);
	Protocol::S_SET_DATA pkt;
	pkt.set_object_id(object->GetObjectInfo()->object_id());
	pkt.set_datatype(Protocol::EPlayerData::DATA_IDLE);
	pkt.set_objecttype(object->GetObjectInfo()->objtype());
	
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer);

	
}

void Room::SetCreatureIdleFromAttack(CreatureRef object, int attackSequence)
{
	if (object->GetObjectInfo()->state() != Protocol::STATE_ATTACK)
		return;

	if (object == nullptr)
		return;

	if (attackSequence != object->GetSAttackComponent()->GetAttackSequence())
		return;
	std::cout << "Idle From Attack" << std::endl;
	SetCreatureIdle(object, Protocol::STATE_ATTACK);
}

void Room::GiveQuestRewards(PlayerRef player, int32 questId)
{    
	shared_ptr<FQuestInfo> questInfo = GDataManager->GetQuestData(questId);
	// 1. 경험치 늘리기
	PTR_CAST(PlayerStatComponent, player->GetStatComponent())->IncreaseExp(questInfo->exp);

	// 2. 골드 늘리기
	player->GetInventory()->AddGold(questInfo->gold);

	// 3. 아이템 보상 주기

	const vector<shared_ptr<FQuestRewardInfo>>& rewards = GDataManager->GetQuestRewardData(questId);

	for (int i = 0; i < rewards.size(); i++)
	{
		AddItem(rewards[i]->itemType, rewards[i]->itemId, rewards[i]->itemCount, player);
	}

}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		PlayerRef player = PTR_CAST(Player, item.second);
		if (player->GetObjectInfo()->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}

void Room::CreateSpawner()
{
	//Temp - 지금은 하드코딩으로 생성 중.
	// 나중엔 Room (Map)마다 가지고 있는 스포너의 정보 (위치, 소환할 몬스터)를 데이터로 가지고 있는다.
	// Room은 그 데이터 데이터에 맞게 스포너 생성

	vector<SpawnerInfo> v = GDBManager->GetSpawnerData();
	//cout << "
	for (auto& value : v)
	{
		// 1. 스포너 클래스 자체 생성
		// 2. 소속된 Room, id, 위치 초기화
		shared_ptr<MonsterSpawner> spawner = make_shared<MonsterSpawner>(value.x, value.y, value.z);
		cout << "Set Spawner ----- > " << value.id << endl;;
		spawner->room = GetRoomRef();
		spawner->id = value.id;
		_spawners[value.id] = spawner;
	
		
		// 3. 스포너에 소속된 몬스터 소환
		stringstream sstream(value.monsters);
		cout << "monsters -> " << value.monsters << endl;;
		string stringId;
		while (sstream >> stringId)
		{
			int id = stoi(stringId);
			cout << "Spawn : " << id << endl;
			spawner->SpawnMonster(id);
		}
		// 4. 일정 주기로 몬스터 리스폰 하도록 함수를 걸어준다.
		Respawn(value.id);
	}

	


	

	
	
}
