#include "pch.h"
#include "Creature.h"
#include "StatComponent.h"
#include "Room.h"

Creature::Creature()
{
	
}

Creature::~Creature()
{
}

void Creature::Attack(CreatureRef target)
{
}

void Creature::OnDamaged(float damage, CreatureRef attacker)
{
	if(objectInfo->state() == Protocol::STATE_DEAD)
		return;

	// 1. 상태를 Damaged로 바꾸고
	if(objectInfo->state() != Protocol::STATE_SKILL)
	{
		objectInfo->set_state(Protocol::STATE_DAMAGED);
	}
	// 2. n초 후에 Idle 실행
	GetRoom()->DoTimer(damagedToIdleTick, &Room::SetCreatureIdle, PTR_CAST(Creature, THIS), Protocol::STATE_DAMAGED);
	GetRoom()->HandleDamaged(PTR_CAST(Creature, THIS), attacker->GetObjectInfo()->object_id(), damage);
}

void Creature::Die(CreatureRef attacker)
{
	std::cout << "Creature Die" << std::endl;
	objectInfo->set_state(Protocol::STATE_DEAD);
	GetRoom()->HandleDie(PTR_CAST(Creature, THIS));

}

void Creature::SendStatPacket()
{
	statComponent->SendStatPacket();
}

void Creature::SendAttackPacket(int attackSequence, Vec3 rotation)
{
	Protocol::S_ATTACK attackPkt;
	Protocol::ObjectInfo* info = attackPkt.mutable_info();
	attackPkt.set_attack_sequence(attackSequence);
	attackPkt.set_x(rotation.x);
	attackPkt.set_y(rotation.y);
	attackPkt.set_z(rotation.z);
	info->CopyFrom(*objectInfo);

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(attackPkt);
	GetRoom()->Broadcast(sendBuffer);
}


