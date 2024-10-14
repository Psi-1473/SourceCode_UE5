#include "pch.h"
#include "BattleManager.h"
#include "MatrixHelper.h"
#include "BaseObject.h"
#include "Creature.h"
#include "ColliderUtils.h"

shared_ptr<BattleManager> GBattleManager = make_shared<BattleManager>();


void BattleManager::AttackHitCheck(const AttackParam& param, const unordered_map<uint64, CreatureRef>& objects)
{
	auto attacker = param.attacker;
	Vec3 attackerRotation = param.attackerRotation;
	Vec3 offset = param.offset;
	Vec3 range = param.range;
	string hitType = param.hitType;
	int32 power = param.power;

	Vec3 attackPos = GetAttackPos(attacker, attackerRotation, offset);

	shared_ptr<Collider> checker = ColliderUtils::CreateCollider(attackPos, range, attackerRotation, hitType);
	for (auto& targetInfo : objects)
	{
		auto target = targetInfo.second;

		if (attacker == target)
			continue;

		if (target->GetObjectInfo()->state() == Protocol::STATE_DEAD)
			continue;

		Protocol::ObjectInfo* targetInfo = target->GetObjectInfo();
		float x = targetInfo->x();
		float y = targetInfo->y();
		float z = targetInfo->z();

		target->GetBoxCollider()->SetCenterPoint(Vec3(x, y, z));
		bool isHit = checker->IsColliding(target->GetBoxCollider());
		if (isHit)
		{
			// 지금 몬스터가 방어력이 없어서 일단 공격력으로만 데미지 전달하기
			//int power = attackerPower
			int totalPower = GetTotalAttackPower(power);
			target->OnDamaged(totalPower, PTR_CAST(Creature, attacker));
			std::cout << "Object Hit!" << std::endl;
		}

	}

}

const Vec3& BattleManager::GetAttackPos(shared_ptr<BaseObject> attacker, Vec3 attackerRotation, Vec3 attackOffset)
{
	Vec3 forwardVector = MatrixHelper::GetForwardVector(attackerRotation);
	Vec3 rightVector = MatrixHelper::GetRightVector(attackerRotation);
	Vec3 upVector = MatrixHelper::GetUpVector(attackerRotation);

	Protocol::ObjectInfo* attackerInfo = attacker->GetObjectInfo();
	float posX = attackerInfo->x() + (forwardVector.x * attackOffset.x) + (rightVector.x * attackOffset.y) + (upVector.x * attackOffset.z);
	float posY = attackerInfo->y() + (forwardVector.y * attackOffset.x) + (rightVector.y * attackOffset.y) + (upVector.y * attackOffset.z);
	float posZ = attackerInfo->z() + (forwardVector.z * attackOffset.x) + (rightVector.z * attackOffset.y) + (upVector.z * attackOffset.z);

	return Vec3(posX, posY, posZ);
}

int32 BattleManager::GetTotalAttackPower(int32 basePower)
{
	int minPower = basePower - (basePower * 0.2f);
	int maxPower = basePower + (basePower * 0.1f);
	int power = Utils::GetRandom(minPower, maxPower);

	return power;
}
