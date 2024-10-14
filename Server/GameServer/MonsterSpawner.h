#pragma once
#include "Geometry.h"

class MonsterSpawner : public enable_shared_from_this<MonsterSpawner>
{
public:
	MonsterSpawner(float x, float y, float z)
	{
		position = Vec3(x, y, z);
	}
	void SpawnMonster(int32 templateId);
	void SpawnMonster(MonsterRef mob);
	void Respawn();

	Vec3 GetRandomPosFromSpawner();

public:
	float respawnTick = 10000.f;
	weak_ptr<class Room> room;
	vector<MonsterRef> monsterList;
	Vec3 position;
	uint64 id;
};

