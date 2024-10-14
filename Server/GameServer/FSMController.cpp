#include "pch.h"
#include "FSMController.h"
#include "Player.h"
#include "Room.h"
#include "Geometry.h"
#include "MapManager.h"
#include "MatrixHelper.h"
#include "MonsterSpawner.h"
#include "AttackComponent.h"

void FSMController::UpdateIdle()
{
	
	// 1. Ÿ�� ���� Ž��
	//cout << "Idle Update" << endl;
	target = FindTarget();
	// 2. Ÿ���� ������ �߰�
	if (target)
	{
		SetState(Protocol::STATE_RUN);
		//std::cout << " *AI Update - IDLE(Next : Target)" << std::endl;
	}
	else
	{
		//std::cout << " *AI Update - IDLE(Next : Patrol)" << std::endl;
		isPatrolMode = true;
		patrolPos = GetPatrolPos();
		SetState(Protocol::STATE_MOVE);
		// 3. Ÿ���� ������ ���� �ϵ��� ����
	}
	
}

void FSMController::UpdateMove()
{
	//std::cout << " *AI Update - Move" << std::endl;
	// ���� or Ÿ���� ���� �� �� ���·� ���´�
	// ���� ���� ��)
	//   1. ���߿��� Ÿ���� ã���� �Ѿƾ� �ϴ� Ÿ�� Ž���� �õ�
	//cout << "Move Update" << endl;
	if (isPatrolMode)
	{
		target = FindTarget();
	}

	// Ÿ���� �Ѵ� ���� ��)
	if (target)
	{
		SetState(Protocol::STATE_RUN);
		isPatrolMode = false;
		
		//   1. Ȥ�ö� �Ÿ��� �ʹ� �־����� �׿� �°� ó���ϰ� Idle ���·�
		Vec3 playerPos = Vec3(*target->GetObjectInfo());
		Vec2Int gridIndex = GMapManager->GetGridPosFromObjPos(playerPos);
		Vec3 targetPos = GMapManager->GetWorldPosFromGrid(gridIndex);
		float dist = targetPos.distanceTo(Vec3(*owner->GetObjectInfo()));

		if (dist >= chaseDistance)
		{
			target = nullptr;
			isPatrolMode = false;
			SetState(Protocol::STATE_IDLE);
			//std::cout << "  *Move AI End (LongDistance)" << dist << " <-> " << chaseDistance  << std::endl;
			return;
		}

		// 2. Ÿ���� �߰� ������ ���� ���� �ְ� ���� ���� �Ÿ����� �ָ� ������ �߰�
		if (dist > attackDistance)
		{
			FindPathAndMove(targetPos);
			return;
		}

		// 3. ����� �����ٸ� ����

		{
			SetState(Protocol::STATE_ATTACK);
			return;
		}
	}
	else if (isPatrolMode)	//   2. Ÿ���� ���� ���� ����� ���� ����
	{
		SetState(Protocol::STATE_MOVE);
		FindPathAndMove(patrolPos);
		return;
	}
	else
	{
		// Target�� ���� ���� ��嵵 �ƴ϶�� �̵� ���� ����

		SetState(Protocol::STATE_IDLE);
	}
}

void FSMController::UpdateAttack()
{
	// (1) ���� �̻� �ɷ������� ���
	// (2) ���߿��� ���� ���� ���� �ð��� �����ͷ� �ּ� �� �ð� ���� ĵ�� ���Ѵٸ� ��ҽ�Ű��
	//std::cout << "  *Attack AI Start" << std::endl;
	// 1. Ÿ���� ��ȿ�Ѱ�?
	//cout << "Attack Update" << endl;
	if (target == nullptr)
	{
		//std::cout << "Attack - Target Null" << std::endl;
		SetState(Protocol::STATE_IDLE);
		return;
	}

	Vec3 targetPos = Vec3(*target->GetObjectInfo());
	float dist = targetPos.distanceTo(Vec3(*owner->GetObjectInfo()));
	if (dist > attackDistance)
	{
		//std::cout << "Attack - LongDistance - " << dist << std::endl;
		GRoom->DoAsync(&Room::SetCreatureIdle, PTR_CAST(Creature, owner), Protocol::STATE_ATTACK);
		return;
	}

	//cout << " *********Monster Pos = (" << owner->GetObjectInfo()->x() << ", " << owner->GetObjectInfo()->y() << ", " << owner->GetObjectInfo()->z() << ")" << endl;
	if(owner->GetSAttackComponent()->GetCanAttack() == false)
	{
		//std::cout << "canAttack False " << std::endl;
		return;
	}

	//std::cout << "Attack - Success" << std::endl;
	owner->GetSAttackComponent()->SetCanAttack(false);
	// time To Idle, time To Attack ���� ����

	owner->Attack(target);
	
	// ��ų ���
}

void FSMController::UpdateDead()
{
}

Vec3 FSMController::GetPatrolPos()
{
	if(owner->GetSpawner() == nullptr)
		return Vec3(*owner->GetObjectInfo());
	Vec3 pos = owner->GetSpawner()->GetRandomPosFromSpawner();

	return pos;
}

bool FSMController::FindPathAndMove(const Vec3 destPos)
{
	// 1. ��ã��
	vector<Vec3> path = GMapManager->FindPath(Vec3(*owner->GetObjectInfo()), destPos);
	// ���� ��ã�Ұų� �ʹ� �ָ� ����
	if (path.size() < 2)// || path.size() > 15)
	{
		//std::cout << "Path Size : " << path.size() << std::endl;
		target = nullptr;
		isPatrolMode = false;
		SetState(Protocol::STATE_IDLE);
		return false;
	}

	// 3. ���� ������Ʈ ��ǥ�� �������� �����Ű��
	//std::cout << "Move Success!" << std::endl;
	owner->Move(path[1]);
	// 4. Ŭ������ ���� ��Ų ��ǥ�� �̵� ��Ű��� ��Ŷ ������
	owner->BroadcastMove();
	return true;
}
