#pragma once
#include "FSMController.h"
class MonsterAIController : public FSMController
{
	// �������� ���� AI�� ���� �� ����� Ŭ����
	// 1. �����ڿ��� id�� ���� �ִ� ���� �Ÿ�, Ž�� �Ÿ�, ���� �Ÿ� ���� ������ �޴´�.
	// 2. ��ų�̳� ���� �����͸� �޴´�.

public:
	MonsterAIController(shared_ptr<class Monster> newOwner);
	void SetState(Protocol::EState state) override;
	PlayerRef FindTarget() override;


private:
	PlayerRef FindTargetForMonster();
};

