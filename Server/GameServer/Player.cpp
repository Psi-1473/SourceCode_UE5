#include "pch.h"
#include "Player.h"
#include "DataManager.h"
#include "BoxCollider.h"
#include "Inventory.h"
#include "Room.h"
#include "StatComponent.h"
#include "PlayerStatComponent.h"
#include "SkillComponent.h""
#include "QuickSlotComponent.h""
#include "QuestComponent.h""
#include "AttackComponent.h""

Player::Player()
{
	objectInfo = new Protocol::ObjectInfo();
	boxCollider = make_shared<BoxCollider>();
	statComponent = make_shared<PlayerStatComponent>();
	skillComponent = make_shared<SkillComponent>();
	quickSlotComponent = make_shared<QuickSlotComponent>();
	questComponent = make_shared<QuestComponent>();
	attackComponent = make_shared<AttackComponent>();
	inventory = make_shared<Inventory>();

	boxCollider->SetSize(Vec3(80.f, 80.f, 160.f));
	boxCollider->SetRotation(Vec3(0.f, 0.f, 0.f));
	damagedToIdleTick = 650;

	
}

Player::~Player()
{
	cout << "Player Destruct " << endl;
	//delete objectInfo;
}

void Player::SetPlayerInfo(const UserBaseInfo& uBase, const CharacterBaseInfo& cBase)
{
	skillComponent->Init(PTR_CAST(Player, THIS));
	inventory->Init(PTR_CAST(Player, THIS));
	quickSlotComponent->Init(PTR_CAST(Player, THIS));
	questComponent->Init(PTR_CAST(Player, THIS));
	attackComponent->Init(PTR_CAST(Player, THIS));
	statComponent->Init(PTR_CAST(Player, THIS), uBase.level);
	//inventory->SetGold(cBase.gold);
	dbId = uBase.characterId;
	playerName = uBase.characterName;
	objectInfo->set_objtype(uBase.characterType);
	objectInfo->set_state(Protocol::STATE_IDLE);
	objectInfo->set_x(cBase.x);
	objectInfo->set_y(cBase.y);
	objectInfo->set_z(cBase.z);
	
	statComponent->GetStatInfo()->set_hp(cBase.hp);
	statComponent->GetStatInfo()->set_mp(cBase.mp);
	statComponent->GetStatInfo()->set_exp(cBase.exp);

	skillComponent->SetSkillPoint(cBase.skillPoint);
}


void Player::Attack(CreatureRef target)
{
	/* 서버에서 Attack 판정해서 공격하는 함수*/
}

void Player::Attack(const Protocol::C_ATTACK& pkt)
{
	/* 클라이언트에서 Attack 패킷을 받아 공격하는 함수*/
	int sequence = pkt.attack_sequence();
	Vec3 rotation = Vec3(pkt.x(), pkt.y(), pkt.z());

	attackComponent->PlayerAttack(rotation, sequence);
}

void Player::UseSkill(const Protocol::C_USESKILL& pkt)
{
	// 1. 스킬 사용 가능한가 확인
	int skillId = pkt.skillid();
	int skillLevel = skillComponent->GetSkillLevel(pkt.skillid());
	int mp = GDataManager->GetSkillMp(GetObjectInfo()->objtype(), pkt.skillid(), skillLevel);
	
	attackComponent->UseSkill(skillId, skillLevel, mp, Vec3(pkt.x(), pkt.y(), pkt.z()));
}


void Player::OnDamaged(float damage, CreatureRef attacker)
{
	Creature::OnDamaged(damage, attacker);
	statComponent->DecreaseHp(damage, attacker);
}





