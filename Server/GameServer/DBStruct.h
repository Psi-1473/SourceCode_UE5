#pragma once
#include <string>
#include "Protocol.pb.h"

using namespace std;

struct UserBaseInfo
{
	uint64 userId;
	uint64 characterId;
	string characterName;
	int32 level;
	Protocol::ECharacterType characterType;

	UserBaseInfo() {}
	UserBaseInfo(uint64 uId, uint64 cId, string name, int32 lvl, Protocol::ECharacterType type)
	: userId(uId), characterId(cId), characterName(name), level(lvl), characterType(type) { }
};

struct CharacterBaseInfo
{
	uint64 id;
	int32 hp;
	int32 mp;
	float x;
	float y;
	float z;
	int32 exp;
	int32 skillPoint;
	int32 gold;

	CharacterBaseInfo() {}
	CharacterBaseInfo(uint64 cId, int32 hp, int32 mp, float x, float y, float z, int32 exp, int32 skillPoint, int32 gold)
		: id(cId), hp(hp), mp(mp), x(x), y(y), z(z), exp(exp), skillPoint(skillPoint), gold(gold) { }
};

struct CharacterAttackInfo
{
	int sequence = 0;
	float offsetX = 0;
	float offsetY = 0;
	float offsetZ = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float timeToHit = 0;
	float comboStart = 0;
	float comboEnd = 0;
	float timeToIdle = 0;
	float damage = 0;
	string colliderType = "";

	CharacterAttackInfo() {}
	CharacterAttackInfo(int seq, float offsetX, float offsetY, float offsetZ, float x, float y, float z, float hit, float start, float end, float idle, float damage, string type)
	: sequence(seq), offsetX(offsetX), offsetY(offsetY), offsetZ(offsetZ), x(x), y(y), z(z), timeToHit(hit), comboStart(start), comboEnd(end),
	timeToIdle(idle), damage(damage), colliderType(type) { }
};

struct MonsterAttackInfo
{
	int sequence = 0;
	float rangeOffset = 0;
	float timeToHit = 0;
	float timeToIdle = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	string colliderType = "";
	MonsterAttackInfo() {}
	MonsterAttackInfo(int seq, float range, float x, float y, float z, float hit, float idle, string type)
	: sequence(seq), rangeOffset(range), x(x), y(y), z(z), timeToHit(hit), timeToIdle(idle), colliderType(type)	{}
};

struct SkillHitInfo
{
	int sequence = 0;
	float offsetFront = 0;
	float offsetUp = 0;
	float offsetRight = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float timeToHit = 0;
	float timeToIdle = 0;
	float coolDown = 0;
	string colliderType = "";

	SkillHitInfo() {}
	SkillHitInfo(int seq, float front, float up, float right, float x, float y, float z, float hit, float idle, float cool, string type)
		: sequence(seq), offsetFront(front), offsetUp(up), offsetRight(right), x(x), y(y), z(z), timeToHit(hit),
		timeToIdle(idle), coolDown(cool), colliderType(type) { }

};

struct SkillDataByLevel
{
public:
	int classId;
	int skillId;
	array<int, 11> levelData;

	SkillDataByLevel() {}
	SkillDataByLevel(int cId, int sId, const array<int, 11> arr)
	: classId(cId), skillId(sId), levelData(arr) {}
};
struct BaseStatInfo
{
	int level = 0;
	int hp = 0;
	int mp = 0;
	int attack = 0;
	int defense = 0;
	int exp = 0;

	BaseStatInfo() {}
	BaseStatInfo(int lvl, int hp, int mp, int atk, int dfs, int exp)
		: level(lvl), hp(hp), mp(mp), attack(atk), defense(dfs), exp(exp) {}
};

struct CharacterStatInfo : public BaseStatInfo
{

	CharacterStatInfo() {}
	CharacterStatInfo(int lvl, int hp, int mp, int atk, int dfs, int exp)
	: BaseStatInfo(lvl, hp, mp, atk, dfs, exp) {}

};

struct MonsterStatInfo : public BaseStatInfo
{
	int id = 0;
	string name = "";

	MonsterStatInfo() {};
	MonsterStatInfo(int id, int lvl, string name, int hp, int atk, int dfs, int exp)
	: BaseStatInfo(lvl, hp, mp, atk, dfs, exp), id(id), name(name) {}
};

struct SpawnerInfo
{
	int id;
	int roomId;
	float x;
	float y;
	float z;
	string monsters;

	SpawnerInfo() {};
	SpawnerInfo(int id, int room, float x, float y, float z, string mobs)
	: id(id), roomId(room), x(x), y(y), z(z), monsters(mobs) { }
};

struct ItemInfo
{
public:
	//uint64 dataId; //DB에 새로 데이터를 만들 때 생성해 줄 고유한 아이템의 ID -> (ex. "이 게임에서  1033102 번 째로 생성된 Id가 1033102인 아이템은 '목검'이야"
	uint32 templateId; // DB에 존재하는 이 아이템의 데이터 ID (ex. '목검'의 데이터를 DB에서 조회하기 위해서는 무기 데이터의 12번 아이디로 접근해야 해)
	string itemName;
	string itemDesc;
	Protocol::EItemGrade grade;
	uint32 power; // (무기 - 공격력, 방어구 - 방어력, 포션 - 회복량)
	uint32 buyPrice;
	uint32 sellPrice;

	ItemInfo() {};
	ItemInfo(uint32 tId, const string& name, const string& desc, Protocol::EItemGrade grade, uint32 power, uint32 buy, uint32 sell)
	:templateId(tId), itemName(name), itemDesc(desc), grade(grade), power(power), buyPrice(buy), sellPrice(sell)
	{}
};

struct WeaponInfo : public ItemInfo
{
	uint32 usableLevel;
	Protocol::ECharacterType characterType;
	Protocol::EWeaponData weaponType;

	WeaponInfo() {};
	WeaponInfo(uint32 tId, const string& name, const string& desc, Protocol::EItemGrade grade, uint32 power, uint32 buy, uint32 sell, uint32 usable, Protocol::EWeaponData wType, Protocol::ECharacterType type)
	: ItemInfo(tId, name, desc, grade, power, buy, sell), usableLevel(usable), weaponType(wType), characterType(type)
	{ }
};

struct ArmorInfo : public ItemInfo
{
	uint32 usableLevel;
	Protocol::ECharacterType characterType;
	Protocol::EArmorData armorType;
	ArmorInfo() {};
	ArmorInfo(uint32 tId, const string& name, const string& desc, Protocol::EItemGrade grade, uint32 power, uint32 buy, uint32 sell, uint32 usable, Protocol::EArmorData aType, Protocol::ECharacterType type)
		: ItemInfo(tId, name, desc, grade, power, buy, sell), usableLevel(usable), armorType(aType), characterType(type)
	{ }
};

struct ConsumableInfo : public ItemInfo
{
	Protocol::EHealType healType;
	ConsumableInfo() {};
	ConsumableInfo(uint32 tId, const string& name, const string& desc, Protocol::EItemGrade grade, uint32 power, uint32 buy, uint32 sell, Protocol::EHealType heal)
		: ItemInfo(tId, name, desc, grade, power, buy, sell), healType(heal)
	{ }
};

struct FQuestInfo
{
	int32 questId;
	Protocol::EQuestGrade questGrade;
	int32 startNpc;
	int32 completeNpc;
	int32 level;
	vector<int32> linkedQuests;
	int32 exp;
	int32 gold;

	FQuestInfo() {};
	FQuestInfo(int32 id, Protocol::EQuestGrade grade, int32 start, int32 complete, int32 lvl, const vector<int32>& linked, int32 exp, int32 gold)
	: questId(id), questGrade(grade), startNpc(start), completeNpc(complete), level(lvl), linkedQuests(linked), exp(exp), gold(gold) {}
};

struct FQuestTargetInfo
{
	int32 questId;
	int32 targetIndex;
	Protocol::ETargetType targetType;
	int32 targetId;
	int32 targetNumber;

	FQuestTargetInfo() {};
	FQuestTargetInfo(int32 id, int32 index, Protocol::ETargetType type, int32 tId, int32 tNumber)
	: questId(id), targetIndex(index), targetType(type), targetId(tId), targetNumber(tNumber){}
};

struct FQuestRewardInfo
{
	int32 questId;;
	Protocol::EItemType itemType;
	int32 itemId;
	int32 itemCount;

	FQuestRewardInfo() {};
	FQuestRewardInfo(int32 qId, Protocol::EItemType type, int32 iId, int32 count)
		: questId(qId), itemType(type), itemId(iId), itemCount(count){}
};