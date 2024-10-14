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
	// 1. �� ����
	// 2. ���� �ʱ�ȭ
	// 3. ���� ��ġ
	MonsterRef mob = ObjectUtils::CreateMonster(id);
	monsterList.push_back(mob);
	Vec3 spawnPos = GetRandomPosFromSpawner();
	mob->SetTemplateId(templateId);
	mob->SetMonsterInfo(spawnPos, shared_from_this());
	room.lock()->SpawnMonster(mob, true);
}

void MonsterSpawner::SpawnMonster(MonsterRef mob)
{
	// 1. mob�� ���� �ʱ�ȭ
	// 2. �ٽ� ���� ��ġ
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

	std::random_device rd; // �õ� ������
	std::mt19937 gen(rd()); // Mersenne Twister ����

	// ���� ���� ������ (1���� 6������ �յ� ����)
	std::uniform_int_distribution<> dis_int(-4, 4);
	std::uniform_int_distribution<> dis_int2(-4, 4);


	// ���⸦ Spawner ��ǥ��
	int newX = posGrid.x + dis_int(gen);
	int newY = posGrid.y + dis_int(gen);

	Vec2Int newPosGrid(newX, newY);

	Vec3 patrolPos = GMapManager->GetWorldPosFromGrid(newPosGrid);
	return patrolPos;
}

