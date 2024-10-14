#include "pch.h"
#include "AttackComponent.h"
#include "DataManager.h"
#include "Player.h"
#include "Room.h"

#include "StatComponent.h"
#include "SkillComponent.h"

AttackComponent::AttackComponent()
{
	attackSequence = 0;
	canAttack = true;
}

AttackComponent::~AttackComponent()
{
}

void AttackComponent::PlayerAttack(const Vec3& rotation, int32 sequence)
{
	auto atkInfo = GDataManager->GetCharacterAttackInfo(owner.lock()->GetObjectInfo()->objtype(), sequence);
	if (IsAttackable(atkInfo, sequence) == false)
		return;

	attackSequence = sequence;
	canAttack = false;
	owner.lock()->GetObjectInfo()->set_state(Protocol::STATE_ATTACK);

	HitCheck(atkInfo, rotation, sequence);
	SendAttackPacket(sequence, rotation);
}

void AttackComponent::MonsterAttack(int32 monsterId, const Vec3& rotation, int32 sequence)
{
	shared_ptr<MonsterAttackInfo> atkInfo = GDataManager->GetMonsterAttackInfo(monsterId, sequence);

	AttackParam param(PTR_CAST(BaseObject, owner.lock()),
		rotation,
		Vec3(atkInfo->rangeOffset, 0.f, 50.f),
		Vec3(atkInfo->x, atkInfo->y, atkInfo->z),
		atkInfo->colliderType,
		10);

	// 2. 히트 판정
	{
		owner.lock()->GetRoom()->DoTimer(1000 * atkInfo->timeToHit, &Room::HitCheck, param);
	}

	// 3. SetIdle
	{
		owner.lock()->GetRoom()->DoTimer(1000 * atkInfo->timeToIdle, &Room::SetCreatureIdle, PTR_CAST(Creature, owner.lock()), Protocol::STATE_ATTACK);
	}


	SendAttackPacket(sequence, rotation);
}

void AttackComponent::UseSkill(int32 skillId, int32 skillLevel, int32 mp, const Vec3& rotation)
{
	if (IsSkillUsable(skillId, mp) == false)
		return;

	// 2. 스탯 및 상태처리

	owner.lock()->GetStatComponent()->DecreaseMp(mp);
	owner.lock()->GetObjectInfo()->set_state(Protocol::STATE_SKILL);

	// 3. 히트 판정
	unordered_map<int, shared_ptr<SkillHitInfo>> hitInfo = GDataManager->GetSkillHitInfo(owner.lock()->GetObjectInfo()->objtype()).at(skillId);
	HitCheck(hitInfo, skillId, skillLevel, rotation);
	ApplySkillCooldown(skillId, hitInfo[1]->coolDown);
	SendSkillPacket(skillId);
}

bool AttackComponent::IsSkillUsable(int32 skillId, int32 mp)
{
	auto player = PTR_CAST(Player, owner.lock());

	if(player == nullptr)
		return false;
	if (mp > player->GetStatComponent()->GetStatInfo()->mp())
		return false;

	if (player->GetObjectInfo()->state() == Protocol::STATE_SKILL)
		return false;

	if (player->GetSkillComponent()->GetSkillUsable(skillId) == false)
		return false;


	return true;
}
bool AttackComponent::IsAttackable(shared_ptr<CharacterAttackInfo> info, int32 sequence)
{
	if (info == nullptr)
		return false;

	if (sequence > GetMaxCombo())
		return false;

	if ((sequence - attackSequence != 1) || canAttack == false)
	{
		auto room = owner.lock()->GetRoom();
		room->DoAsync(&Room::SetCreatureIdleFromAttack, PTR_CAST(Creature, owner.lock()), sequence);
		room->DoAsync(&Room::SetCreatureAttackable, PTR_CAST(Creature, owner.lock()), true, sequence);
		return false;
	}


	return true;
}
void AttackComponent::HitCheck(const unordered_map<int, shared_ptr<struct SkillHitInfo>>& info, int32 skillId, int32 skillLevel, Vec3 rotation)
{
	unordered_map<int, shared_ptr<SkillHitInfo>> hitInfo = GDataManager->GetSkillHitInfo(owner.lock()->GetObjectInfo()->objtype()).at(skillId);
	int dmgMultiplier = GDataManager->GetSkillDamage(owner.lock()->GetObjectInfo()->objtype(), skillId, skillLevel);

	// for문 돌면서 hitInfo에 애들 다 HitCheck n초 뒤에 하도록 적용시키기
	for (int i = 1; i <= hitInfo.size(); i++)
	{
		shared_ptr <SkillHitInfo> hit = hitInfo[i];
		int power = owner.lock()->GetStatComponent()->GetStatInfo()->atk();
		power = power + (power * ((float)dmgMultiplier / 100));

		AttackParam param(PTR_CAST(BaseObject, owner.lock()),
			Vec3(rotation.x, rotation.y, rotation.z),
			Vec3(hit->offsetFront, hit->offsetUp, hit->offsetRight),
			Vec3(hit->x, hit->y, hit->z),
			hit->colliderType,
			power);


		owner.lock()->GetRoom()->DoTimer(1000.f * hit->timeToHit, &Room::HitCheck, param);
		if (i == 1)
		{
			// Idle 등록하기
			owner.lock()->GetRoom()->DoTimer(1000.f * hit->timeToIdle, &Room::SetCreatureIdle, PTR_CAST(Creature, owner.lock()), Protocol::STATE_SKILL);
		}
	}
}
void AttackComponent::HitCheck(shared_ptr<struct CharacterAttackInfo> info, const Vec3& rotation, int32 sequence)
{
	int power = owner.lock()->GetStatComponent()->GetStatInfo()->atk();
	power = power + (power * (info->damage / 100));
	AttackParam param(PTR_CAST(BaseObject, owner.lock()),
		Vec3(rotation.x, rotation.y, rotation.z),
		Vec3(info->offsetX, info->offsetY, info->offsetZ),
		Vec3(info->x, info->y, info->z),
		info->colliderType,
		power);
	// 2. n 초 후에 히트 대상 정보 Broadcast
	owner.lock()->GetRoom()->DoTimer((1000.f * info->timeToHit), &Room::HitCheck, param);
	// 3. m 초 후에 canAttack = true;
	owner.lock()->GetRoom()->DoTimer((1000.f * info->comboStart), &Room::SetCreatureAttackable, PTR_CAST(Creature, owner.lock()), true, sequence);
	owner.lock()->GetRoom()->DoTimer((1000.f * info->comboEnd), &Room::SetCreatureAttackable, PTR_CAST(Creature, owner.lock()), false, sequence);
	// 4. m 초 후에 해당 캐릭터 Idle로 설정하라고 BroadCast
	owner.lock()->GetRoom()->DoTimer((1000.f * info->timeToIdle), &Room::SetCreatureIdleFromAttack, PTR_CAST(Creature, owner.lock()), sequence);

}
void AttackComponent::ApplySkillCooldown(int32 skillId, int32 coolDown)
{
	auto player = PTR_CAST(Player, owner.lock());

	if (player == nullptr)
		return;

	player->GetSkillComponent()->SetSkillUsable(skillId, false);
	SkillComponentRef myComp = player->GetSkillComponent();
	auto myLambda = [skillId, myComp]() {
		myComp->SetSkillUsable(skillId, true);
		cout << skillId << " Skill : Usable! " << endl;
		};


	owner.lock()->GetRoom()->DoTimer(1000.f * coolDown, myLambda);
}
void AttackComponent::SendAttackPacket(int attackSequence, const Vec3& rotation)
{
	Protocol::S_ATTACK attackPkt;
	Protocol::ObjectInfo* info = attackPkt.mutable_info();
	attackPkt.set_attack_sequence(attackSequence);
	attackPkt.set_x(rotation.x);
	attackPkt.set_y(rotation.y);
	attackPkt.set_z(rotation.z);
	info->CopyFrom(*(owner.lock()->GetObjectInfo()));

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(attackPkt);
	owner.lock()->GetRoom()->Broadcast(sendBuffer);
}
void AttackComponent::SendSkillPacket(int skillId)
{
	Protocol::S_USESKILL skillPkt;
	Protocol::ObjectInfo* info = skillPkt.mutable_info();
	info->CopyFrom(*(owner.lock()->GetObjectInfo()));
	skillPkt.set_skillid(skillId);
	int32 coolDown = GDataManager->GetSkillHitInfo(info->objtype()).at(skillId).at(1)->coolDown;
	skillPkt.set_cooldown(coolDown);
	cout << "Use : " << skillId << " Skill" << endl;
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(skillPkt);
	owner.lock()->GetRoom()->Broadcast(sendBuffer);
}
int32 AttackComponent::GetMaxCombo()
{
	return GDataManager->GetCharacterMaxCombo(owner.lock()->GetObjectInfo()->objtype());
}
