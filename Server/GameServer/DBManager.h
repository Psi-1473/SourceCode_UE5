#pragma once
//#include "sql/jdbc/mysql_driver.h"
//#include "sql/jdbc/mysql_connection.h"
//#include "sql/mysql/jdbc.h"

#include "DBStruct.h"
#include <sql/jdbc/mysql_driver.h>
#include <sql/jdbc/mysql_connection.h>
#include <sql/jdbc/cppconn/exception.h>
#include <sql/jdbc/cppconn/statement.h>
#include <sql/jdbc/cppconn/resultset.h>
#include <sql/jdbc/cppconn/prepared_statement.h>

class DBManager : public JobQueue
{
public:
	void ConnectDB();

	/*
		���� �߿� ����Ǿ�� �ϴ� �Լ��� (ex. ������ ȹ��, ���� ���� DB�� ����)
	*/
	void HandleLogin(string id, PacketSessionRef session);
	void HandleLogin2(uint64 userId, PacketSessionRef session);

	void CheckName(PacketSessionRef session, string name);
	void CheckName2(PacketSessionRef session, string name, bool isDuplicated);

	void CreateCharacter(PacketSessionRef session, Protocol::C_CREATECHARACTER pkt);

	void EnterPlayer(PlayerRef player, uint64 characterId, RoomRef room);

	void SetItems(RoomRef room, PlayerRef player);
	void SetSkills(RoomRef room, PlayerRef player);
	void SetSkills2(RoomRef room, PlayerRef player, vector<struct SkillInfo> skills);

	void AddItem1(Protocol::EItemType itemType, uint32 templateId, Protocol::EQuickSlot slot, PlayerRef owner, RoomRef room, int32 count = 1); //DB�� ������ ������ �߰�
	void AddItem2(Protocol::EItemType itemType, uint64 dataId, uint32 templateId, int slotIndex, Protocol::EQuickSlot slot, PlayerRef owner, int32 count = 1, bool sendToClient = true); // ���� �����尡 ������ �Լ�
	
	void RemoveOrDecrement(vector<uint64> ids, vector<uint32> itemCount, PlayerRef player, RoomRef room);
	void RemoveOrDecrementSingle(uint64 dbId, uint32 itemCount, PlayerRef player, RoomRef room);

	void RemoveItemSingle(uint64 dbId, PlayerRef player, RoomRef room);
	void RemoveItemSingle2(uint64 dbId, PlayerRef player);
	void RemoveItem(vector<uint64> ids, PlayerRef player, RoomRef room);
	void RemoveItem2(vector<uint64> ids, PlayerRef player);

	void SetItemCountSingle(uint64 dbId, PlayerRef player, RoomRef room, uint32 itemCount);
	void SetItemCountSingle2(uint64 dbId, PlayerRef player, uint32 itemCount);
	void SetItemCount(vector<uint64> ids, PlayerRef player, RoomRef room, vector<uint32> itemCounts);
	void SetItemCount2(vector<uint64> ids, PlayerRef player, vector<uint32> itemCounts);

	void AddNewSkill(int32 skillId, PlayerRef player);
	void SetSkillLevel(int32 skillId, int32 skillLevel, PlayerRef player);

	void UpdateSkillInMemory(int32 skillId, int32 skillLevel, PlayerRef player);

	void SetQuickSlot(Protocol::EQuickSlot slot, uint64 id, Protocol::EQuickElementType type, PlayerRef player);
	void SetQuickSlot2(Protocol::EQuickSlot slot, uint64 id, Protocol::EQuickElementType type, PlayerRef player);

	void InitEquippedItem(uint64 dataId, PlayerRef player, Protocol::EItemType type, uint32 templateId);
	void EquipItem(uint64 dataId, PlayerRef player, RoomRef room, ItemRef unequipItem = nullptr);
	void EquipItem2(uint64 dataId, PlayerRef player, ItemRef unequipItem);
	void UnEquipItem(uint64 dataId, PlayerRef player, RoomRef room);
	void ItemSlotChange(uint64 itemDbId, int32 slotIndex);

	void LevelUp(PlayerRef player, int32 level);

	/*
		����Ʈ ���� �Լ�
	*/
	
	// 1-1) �÷��̾ ���� ��� ����Ʈ ��������
	void LoadAvailableQuests(PlayerRef player);
	void LoadInProgressQuests(PlayerRef player);

	// 1-2) ���������� �ش� ������ ����Ʈ �����͸� �÷��̾����� �߰�
	void LoadAvailableQuestsByLevel(PlayerRef player, int32 level);

	// 2. �� ��, �÷��̾ ���� ��� ����Ʈ�� �޸𸮿� ����
	void AddAvailableQuestsInMemory(PlayerRef player, const vector<int32>& ids);
	void AddInProgressQuestsInMemory(PlayerRef player, const vector<int32>& ids);
	void LoadQuestTargets(PlayerRef player, vector<int32> ids);

	// Ư�� id�� ����Ʈ �����͸� �÷��̾����� �߰�
	void LoadAvailableQuestsById(PlayerRef player, int32 questId);

	void AcceptNewQuest(PlayerRef player, int32 questId);
	void AbandonQuest(PlayerRef player, int32 questId);
	void CompleteQuest(PlayerRef player, int32 questId);

	void AddQuestTarget(PlayerRef player, int32 questId);

	void AddQuickQuest(PlayerRef player, int32 questId);
	void RemoveQuickQuest(PlayerRef player, int32 questId);
	void LoadQuickQuestList(PlayerRef player);
	void LoadQuickQuestList2(PlayerRef player, const vector<int32> quickIds);


	/* ���� �����, �÷��̾� ���� ���� */
	void SavePlayerInfo(PlayerRef player);
	
public:
	/*
		���� ���� ��, �ʱ�ȭ ���� �Լ���
	*/
	
	void GetCharacterAttackData();
	void GetMonsterAttackData();
	void GetWarriorStatData();
	void GetRogueStatData();
	void GetWizardStatData();
	void GetWarriorSkillHitData();
	void GetRogueSkillHitData();
	void GetWizardSkillHitData();
	void GetWarriorSkillDamageData();
	void GetRogueSkillDamageData();
	void GetWizardSkillDamageData();
	void GetWarriorSkillMpData();
	void GetRogueSkillMpData();
	void GetWizardSkillMpData();

	void GetMonsterStatData();
	void GetWeaponData();
	void GetArmorData();
	void GetConsumableData();
	
	void GetQuestInfoData();
	void GetQuestTargetData();
	void GetQuestRewardData();
	vector<SpawnerInfo> GetSpawnerData();

private:
	// ĳ���� ����â���� ����� �ش� ���� ĳ������ �⺻ ���� ��������
	vector<UserBaseInfo> GetCharacterBaseInfoList(uint64 userId);
	UserBaseInfo GetUserBaseInfo(uint64 characterId);
	CharacterBaseInfo GetCharacterBaseInfo(uint64 characterId);

	bool CreateNewCharacter(uint64 userId, string name, Protocol::ECharacterType type, uint64& characterId);
	void SetNewCharacterInfo(uint64 characterId, Protocol::ECharacterType type);

	// DB �� �޸𸮿� ����Ʈ ������ �߰�
	void AddAvailableQuest(PlayerRef player, int32 questId);
	void AddInProgressQuest(PlayerRef player, int32 questId);

	void RemoveAvailableQuest(PlayerRef player, int32 questId);
	void RemoveInProgressQuest(PlayerRef player, int32 questId);
private:
	sql::mysql::MySQL_Driver* driver;
	unique_ptr<sql::Connection> con;
};

extern shared_ptr<DBManager> GDBManager;
