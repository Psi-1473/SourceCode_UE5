#include "pch.h"
#include "BaseAIController.h"
#include "Room.h"

void BaseAIController::Update()
{
	switch (owner->GetObjectInfo()->state())
	{
	case Protocol::STATE_NONE:
		std::cout << "State None" << std::endl;
		break;
	case Protocol::STATE_IDLE:
		UpdateIdle();
		break;
	case Protocol::STATE_MOVE:
		UpdateMove();
		break;
	case Protocol::STATE_RUN:
		UpdateMove();
		break;
	case Protocol::STATE_ATTACK:
		UpdateAttack();
		break;
	case Protocol::STATE_DAMAGED:
		UpdateDead();
		break;
	default:
		break;
	}

	if(owner->GetRoom() == nullptr)
		return;

	owner->GetRoom()->DoTimer(updateTick, &Room::UpdateTick, static_pointer_cast<BaseObject>(owner));
}

void BaseAIController::SetState(Protocol::EState newState)
{
	if(newState == owner->GetObjectInfo()->state())
		return;

	owner->GetObjectInfo()->set_state(newState);
}
