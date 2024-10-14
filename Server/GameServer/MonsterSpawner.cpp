#include "pch.h"
#include "MonsterSpawner.h"
#include "Monster.h"
#include "MapManager.h"
#include "ObjectUtils.h"
#include "Protocol.pb.h"
#include "Room.h"
#include <random>

void MonsterSpawner::SpawnMonster(int32 templateId)
{
	// 1. 몹 생성
	// 2. 정보 초기화
	// 3. 씬에 배치
	MonsterRef mob = ObjectUtils::CreateMonster(id);
	monsterList.push_back(mob);
	Vec3 spawnPos = GetRandomPosFromSpawner();
	mob->SetTemplateId(templateId);
	mob->SetMonsterInfo(spawnPos, shared_from_this());
	room.lock()->SpawnMonster(mob, true);
}

void MonsterSpawner::SpawnMonster(MonsterRef mob)
{
	// 1. mob의 정보 초기화
	// 2. 다시 씬에 배치
	//InitMonster(mob);
	Vec3 spawnPos = GetRandomPosFromSpawner();
	mob->SetMonsterInfo(spawnPos, shared_from_this());
	room.lock()->SpawnMonster(mob, false);
}

void MonsterSpawner::Respawn()
{
	for (const auto& mob : monsterList)
	{
		if(mob->GetObjectInfo() == nullptr)
			continue;
		if (mob->GetObjectInfo()->state() == Protocol::STATE_DEAD)
		{
			SpawnMonster(mob);
		}
	}

	room.lock()->DoTimer(respawnTick, &Room::Respawn, id);
}

Vec3 MonsterSpawner::GetRandomPosFromSpawner()
{
	Vec2Int posGrid = GMapManager->GetGridPosFromObjPos(position);

	std::random_device rd; // 시드 생성기
	std::mt19937 gen(rd()); // Mersenne Twister 엔진

	// 정수 난수 생성기 (1부터 6까지의 균등 분포)
	std::uniform_int_distribution<> dis_int(-4, 4);
	std::uniform_int_distribution<> dis_int2(-4, 4);


	// 여기를 Spawner 좌표로
	int newX = posGrid.x + dis_int(gen);
	int newY = posGrid.y + dis_int(gen);

	Vec2Int newPosGrid(newX, newY);

	Vec3 patrolPos = GMapManager->GetWorldPosFromGrid(newPosGrid);
	return patrolPos;
}

