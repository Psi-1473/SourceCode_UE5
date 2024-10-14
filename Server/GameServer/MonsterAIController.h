#pragma once
#include "FSMController.h"
class MonsterAIController : public FSMController
{
	// 세부적인 몬스터 AI를 만들 때 사용할 클래스
	// 1. 생성자에서 id를 통해 최대 추적 거리, 탐색 거리, 정찰 거리 등의 정보를 받는다.
	// 2. 스킬이나 공격 데이터를 받는다.

public:
	MonsterAIController(shared_ptr<class Monster> newOwner);
	void SetState(Protocol::EState state) override;
	PlayerRef FindTarget() override;


private:
	PlayerRef FindTargetForMonster();
};

