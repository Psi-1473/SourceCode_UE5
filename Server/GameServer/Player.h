#pragma once
#include "Creature.h"


class GameSession;
class Room;
struct UserBaseInfo;
struct CharacterBaseInfo;
class Player : public Creature
{
public:
	Player();
	virtual ~Player();

	/* 처음 게임 시작 시, 기본적인 캐릭터 정보를 세팅하기 위한 함수 */
	void SetPlayerInfo(const UserBaseInfo& uBase, const CharacterBaseInfo& cBase);

	
	virtual void Attack(CreatureRef target);
	void Attack(const Protocol::C_ATTACK& pkt);
	void UseSkill(const Protocol::C_USESKILL& pkt);
	virtual void OnDamaged(float damage, CreatureRef attacker) override;

public:
	/* Get-Set*/
	uint64 GetDbId() { return dbId; }
	string GetPlayerName() { return playerName; }
	void SetDbId(uint64 value) { dbId = value; }
	void SetPlayerName(string name) { playerName = name; }

	// Component
	SkillComponentRef					GetSkillComponent() { return skillComponent; }
	QuickSlotComponentRef				GetQuickSlotComponent() { return quickSlotComponent; }
	InventoryRef						GetInventory() { return inventory; }
	shared_ptr<class QuestComponent>	GetQuestComponent() { return questComponent; }

private:
	virtual void Update() override {}


public:
	weak_ptr<GameSession> session;

private:
	uint64 dbId;
	string playerName;

	SkillComponentRef					skillComponent;
	QuickSlotComponentRef				quickSlotComponent;
	InventoryRef						inventory;
	shared_ptr<class QuestComponent>	questComponent;


	
};

