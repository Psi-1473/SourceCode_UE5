#pragma once
#include "Geometry.h"



class BattleManager
{

public:
	void AttackHitCheck(const AttackParam& param, const unordered_map<uint64, CreatureRef>& objects);

public:
	const Vec3& GetAttackPos(shared_ptr<BaseObject> attacker, Vec3 attackerRotation, Vec3 attackOffset);
	int32 GetTotalAttackPower(int32 basePower);
};

extern shared_ptr<BattleManager> GBattleManager;
