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
	
	// 1. 타겟 먼저 탐색
	//cout << "Idle Update" << endl;
	target = FindTarget();
	// 2. 타겟이 있으면 추격
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
		// 3. 타겟이 없으면 정찰 하도록 설정
	}
	
}

void FSMController::UpdateMove()
{
	//std::cout << " *AI Update - Move" << std::endl;
	// 정찰 or 타겟을 쫓을 때 이 상태로 들어온다
	// 정찰 중일 때)
	//   1. 도중에라도 타겟을 찾으면 쫓아야 하니 타겟 탐색을 시도
	//cout << "Move Update" << endl;
	if (isPatrolMode)
	{
		target = FindTarget();
	}

	// 타겟을 쫓는 중일 때)
	if (target)
	{
		SetState(Protocol::STATE_RUN);
		isPatrolMode = false;
		
		//   1. 혹시라도 거리가 너무 멀어지면 그에 맞게 처리하고 Idle 상태로
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

		// 2. 타겟이 추격 가능한 범위 내에 있고 공격 가능 거리보단 멀리 있으면 추격
		if (dist > attackDistance)
		{
			FindPathAndMove(targetPos);
			return;
		}

		// 3. 충분히 가깝다면 공격

		{
			SetState(Protocol::STATE_ATTACK);
			return;
		}
	}
	else if (isPatrolMode)	//   2. 타겟이 없는 정찰 모드라면 정찰 실행
	{
		SetState(Protocol::STATE_MOVE);
		FindPathAndMove(patrolPos);
		return;
	}
	else
	{
		// Target도 없고 정찰 모드도 아니라면 이동 상태 종료

		SetState(Protocol::STATE_IDLE);
	}
}

void FSMController::UpdateAttack()
{
	// (1) 상태 이상에 걸려있으면 취소
	// (2) 나중에는 몬스터 공격 판정 시간을 데이터로 둬서 그 시간 전에 캔슬 당한다면 취소시키자
	//std::cout << "  *Attack AI Start" << std::endl;
	// 1. 타겟이 유효한가?
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
	// time To Idle, time To Attack 임의 설정

	owner->Attack(target);
	
	// 스킬 사용
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
	// 1. 길찾기
	vector<Vec3> path = GMapManager->FindPath(Vec3(*owner->GetObjectInfo()), destPos);
	// 길을 못찾았거나 너무 멀면 포기
	if (path.size() < 2)// || path.size() > 15)
	{
		//std::cout << "Path Size : " << path.size() << std::endl;
		target = nullptr;
		isPatrolMode = false;
		SetState(Protocol::STATE_IDLE);
		return false;
	}

	// 3. 몬스터 오브젝트 좌표를 서버에서 변경시키기
	//std::cout << "Move Success!" << std::endl;
	owner->Move(path[1]);
	// 4. 클라한테 변경 시킨 좌표로 이동 시키라고 패킷 보내기
	owner->BroadcastMove();
	return true;
}
