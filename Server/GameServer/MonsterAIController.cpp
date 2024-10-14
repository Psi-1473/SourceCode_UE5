#include "pch.h"
#include "MonsterAIController.h"
#include "Player.h"
#include "Room.h"

MonsterAIController::MonsterAIController(shared_ptr<class Monster> newOwner)
{
	owner = newOwner;
}

void MonsterAIController::SetState(Protocol::EState state)
{
	FSMController::SetState(state);

	switch (state)
	{
		case Protocol::STATE_IDLE:
			updateTick = 250;
			break;
		case Protocol::STATE_MOVE:
			updateTick = 900;
			break;
		case Protocol::STATE_RUN:
			updateTick = 360;
			break;
		case Protocol::STATE_ATTACK:
			updateTick = 800;
			break;
		//case Protocol::STATE_:
		//	break;
	}
}

PlayerRef MonsterAIController::FindTarget()
{
	if(owner->GetRoom() == nullptr)
		return nullptr;


	// �񼱰� �������� �Ǻ��ϰ� �񼱰� ���̸� �������� �ʵ��� ����

	
	return FindTargetForMonster();
}

PlayerRef MonsterAIController::FindTargetForMonster()
{

	// Temp : ��� �÷��̾� ���� �� ������
	PlayerRef target = nullptr;
	unordered_map<uint64, CreatureRef> players = owner->GetRoom()->GetPlayers();
	Vec3 myPos(*owner->GetObjectInfo());
	float minDist = 100000.f;
	for (auto player : players)
	{
		Vec3 targetPos(*player.second->GetObjectInfo());
		float distance = myPos.distanceTo(targetPos);
		if(distance > chaseDistance)
			continue;
		if (distance < minDist)
		{
			target = PTR_CAST(Player, player.second);
			minDist = distance;
		}

		
	}
	
	return target;
}
