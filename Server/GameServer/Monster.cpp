#include "pch.h"
#include "Monster.h"
#include "Player.h"
#include "Geometry.h"
#include "MatrixHelper.h"
#include "Room.h"
#include "BoxCollider.h"
#include "MonsterAIController.h"
#include "DataManager.h"
#include "MapManager.h"
#include "MonsterSpawner.h"
#include "StatComponent.h"
#include "AttackComponent.h"
#include "MonsterStatComponent.h"
#include "PlayerStatComponent.h"
#include "QuestComponent.h"

Monster::Monster()
{
	// 씬에 소환되기 전에 초기화 되어야 할 변수들
	objectInfo = new Protocol::ObjectInfo();
	boxCollider = make_shared<BoxCollider>();
	boxCollider->SetSize(Vec3(40.f, 40.f, 60.f));
	boxCollider->SetRotation(Vec3(0.f, 0.f, 0.f));
	statComponent = make_shared<MonsterStatComponent>();
	attackComponent = make_shared<AttackComponent>();
	damagedToIdleTick = 800;

}

Monster::~Monster()
{
}

void Monster::SetMonsterInfo(Vec3 spawnPos, shared_ptr<class MonsterSpawner> newSpawner)
{
	// 1. templateId를 통한 정보 초기화
	if (ai == nullptr)
	{
		ai = make_shared<MonsterAIController>(static_pointer_cast<Monster>(shared_from_this()));
	}

	statComponent->Init(PTR_CAST(Creature, THIS), templateId);
	attackComponent->Init(PTR_CAST(Creature, THIS));
	// 2. Update 함수 실행시키기
	auto monsterInfo = GetObjectInfo();

	monsterInfo->set_objtype(Protocol::MONSTER);
	monsterInfo->set_x(spawnPos.x);
	monsterInfo->set_y(spawnPos.y);
	monsterInfo->set_z(spawnPos.z + 90);
	monsterInfo->set_state(Protocol::EState::STATE_IDLE);
	SetSpawner(newSpawner);
}

void Monster::Update()
{
	ai->Update();
}

void Monster::Move(const Vec3& pos)
{

	// 나중에 Zone 같은 개념이 추가되면 할 일 추가.
	
	GMapManager->ReleaseGrid(Vec3(*objectInfo));
	objectInfo->set_x(pos.x);
	objectInfo->set_y(pos.y);
	objectInfo->set_z(pos.z + 90); // offset 추가
	//cout << "pos z = " << pos.z << endl;
	GMapManager->OccupyGrid(Vec3(*objectInfo));
	//GMapManager
	// offset을 추가하지 않으면 -> 공격할 때, Distance 확인할 때 오차가 커짐
	// offset을 추가하면 -> 몬스터의 이동 좌표가 이상하게 찍힘
}

void Monster::Attack(CreatureRef target)
{
	std::cout << "Monster Attack !" << std::endl;
	if(target == nullptr)
		return;

	Vec3 dir = Vec3(*target->GetObjectInfo()) - Vec3(*objectInfo);
	Vec3 rotation = MatrixHelper::GetYawFromDirection(dir);

	// id, sequence 1로 잠시 강제 설정
	attackComponent->MonsterAttack(0, rotation, 1);
}

void Monster::OnDamaged(float damage, CreatureRef attacker)
{
	Creature::OnDamaged(damage, attacker);

	statComponent->DecreaseHp(damage, attacker);
	
}

void Monster::Die(CreatureRef attacker)
{
	Creature::Die(attacker);

	auto player = PTR_CAST(Player, attacker);

	if (player == nullptr)
		return;

	player->GetQuestComponent()->TargetUp(templateId, Protocol::TARGET_MONSTER, 1);
	auto attackerStat = PTR_CAST(PlayerStatComponent, player->GetStatComponent());
	attackerStat->IncreaseExp(statComponent->GetStatInfo()->exp());

	GMapManager->ReleaseGrid(Vec3(*objectInfo));

	
}

void Monster::BroadcastMove()
{
	Protocol::S_MOVE pkt;
	Protocol::ObjectInfo* info = new Protocol::ObjectInfo();

	info->CopyFrom(*objectInfo);
	info->set_z(info->z() - 90);
	pkt.set_allocated_info(info);

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	room.load().lock()->Broadcast(sendBuffer);
}

void Monster::Respawn()
{
	spawner.lock()->SpawnMonster(PTR_CAST(Monster, THIS));
}

