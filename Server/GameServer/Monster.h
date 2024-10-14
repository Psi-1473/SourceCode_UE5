#pragma once
#include "Creature.h"

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();

	/* 클라이언트 씬에 스폰하기 전, 몬스터 정보 초기화 */
	void SetMonsterInfo(struct Vec3 spawnPos, shared_ptr<class MonsterSpawner> newSpawner);

	virtual void Update() override;
public:
	void Move(const struct Vec3& pos);
	virtual void Attack(CreatureRef target) override;
	virtual void OnDamaged(float damage, CreatureRef attacker) override;
	virtual void Die(CreatureRef attacker) override;
	void BroadcastMove();
	void Respawn();

public:
	void SetSpawner(shared_ptr<class MonsterSpawner> newSpawner) { spawner = newSpawner; }
	void SetTemplateId(int32 id) { templateId = id; }
	shared_ptr<class MonsterSpawner> GetSpawner() { return spawner.lock(); }

	int32 GetTemplateId() { return templateId; }
private:
	shared_ptr<class MonsterAIController> ai;
	weak_ptr<class MonsterSpawner> spawner;
	int32 templateId;
};

