#pragma once
#include "DBStruct.h"
#include "Protocol.pb.h"
#include "DBManager.h"

class DataManager
{
	friend DBManager;

public:
	void Init();

public:
	// 1. 어택 데이터 읽어와서 가지고 있게 하기
	// 2. 어택 판정할 때, 그 데이터를 토대로 하도록 하기
	const NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillHitInfo>)& GetSkillHitInfo(Protocol::ECharacterType type);
	int GetCharacterMaxCombo(Protocol::ECharacterType type) { return characterAttackData[type].size(); }
	shared_ptr<CharacterStatInfo> GetCharacterStat(Protocol::ECharacterType type, int level) { return characterStat[type][level]; }
	shared_ptr<CharacterAttackInfo> GetCharacterAttackInfo(Protocol::ECharacterType type, int sequence) { return characterAttackData[type][sequence]; }
	shared_ptr<MonsterAttackInfo> GetMonsterAttackInfo(int id, int sequence) { return monsterAttackData[id][sequence]; }
	shared_ptr<MonsterStatInfo> GetMonsterStat(int id) { return monsterStat[id]; }
	shared_ptr<WeaponInfo> GetWeaponData(uint32 templateId) { return weaponData[templateId]; }
	shared_ptr<ArmorInfo> GetArmorData(uint32 templateId) { return armorData[templateId]; }
	shared_ptr<ItemInfo> GetConsumableData(uint32 templateId) { return consumableData[templateId]; }
	int32 GetSkillDamage(Protocol::ECharacterType type, int32 skillId, int32 level) {return skillDamageData[type][skillId]->levelData[level]; }
	int32 GetSkillMp(Protocol::ECharacterType type, int32 skillId, int32 level) {return skillMpData[type][skillId]->levelData[level]; }

	
	shared_ptr<FQuestInfo> GetQuestData(int32 id) { return questData[id]; }
	unordered_map<int, shared_ptr<FQuestTargetInfo>> GetQuestTargetData(int32 id) { return questTargetData[id]; }
	vector <shared_ptr<FQuestRewardInfo>> GetQuestRewardData(int32 questId) { return questRewardData[questId];  }

private:
	NESTED_UNORDERED_MAP(int, int, shared_ptr<CharacterStatInfo>) characterStat;
	unordered_map<int, shared_ptr<MonsterStatInfo>> monsterStat;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<CharacterAttackInfo>) characterAttackData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<MonsterAttackInfo>) monsterAttackData;

	NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillHitInfo>) warriorSkillHitData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillHitInfo>) rogueSkillHitData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillHitInfo>) wizardSkillHitData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillDataByLevel>) skillDamageData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillDataByLevel>) skillMpData;

	unordered_map<int, shared_ptr<WeaponInfo>> weaponData;
	unordered_map<int, shared_ptr<ArmorInfo>> armorData;
	unordered_map<int, shared_ptr<ItemInfo>> consumableData;

	unordered_map<int, shared_ptr<FQuestInfo>> questData;
	NESTED_UNORDERED_MAP(int, int, shared_ptr<FQuestTargetInfo>) questTargetData;
	unordered_map<int, vector<shared_ptr<FQuestRewardInfo>>> questRewardData;
	
};
extern shared_ptr<DataManager> GDataManager;

