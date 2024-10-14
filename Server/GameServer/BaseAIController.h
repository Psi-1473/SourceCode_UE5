#pragma once
#include "BaseObject.h"
#include "Monster.h"
#include "Geometry.h"

class BaseAIController
{
public:
	virtual void Update();
	virtual void SetState(Protocol::EState newState);


protected:
	virtual void UpdateIdle() {}
	virtual void UpdateMove() {}
	virtual void UpdateAttack() {}
	virtual void UpdateDead() {}



protected:
	int32 updateTick = 1000;
	shared_ptr<Monster> owner;
};

