#pragma once
#include "BaseAIController.h"
#include "Geometry.h"

class FSMController : public BaseAIController
{

protected:
	virtual void UpdateIdle() override;
	virtual void UpdateMove() override;
	virtual void UpdateAttack() override;
	virtual void UpdateDead() override;

	virtual PlayerRef FindTarget() abstract;
	virtual Vec3 GetPatrolPos();

	bool FindPathAndMove(const Vec3 destPos);
protected:
	PlayerRef target;
	Vec3 patrolPos;

	float chaseDistance = 800; // �Ѿư� �ִ� �Ÿ�
	float attackDistance = 200; // ���� ������ �Ÿ�
	bool isPatrolMode = false;
};

