#include "pch.h"
#include "StatComponent.h"
#include "Player.h"
#include "Inventory.h"
#include "DataManager.h"
#include "Room.h"

StatComponent::StatComponent()
{
	
}

// Level Up���� �̸� �ٲܱ� �ƴϴ� SetStatByLevel�� ?
void StatComponent::Init(CreatureRef creature, int32 newLevel)
{
	owner = creature;
	SetDefaultStat(newLevel);
	if(statInfo == nullptr)
	{
		statInfo = make_shared<Protocol::StatInfo>();
	}
	statInfo->set_level(newLevel);
	statInfo->set_hp(defaultStat->hp);
	statInfo->set_mp(defaultStat->mp);
	statInfo->set_atk(defaultStat->attack); // ������ �׳� ���� �ߴµ�, ���߿� ���� ���ݷµ� ���ؼ�
	statInfo->set_dfs(defaultStat->defense); // ������ �׳� ���� �ߴµ�, ���߿� �� ���µ� ���ؼ�
}

void StatComponent::SetDefaultStat(int32 level)
{
	
}

void StatComponent::IncreaseHp(int value)
{
	statInfo->set_hp(statInfo->hp() + value);
	if (statInfo->hp() > defaultStat->hp)
	{
		statInfo->set_hp(defaultStat->hp);
	}
	SendStatPacket();
}

void StatComponent::DecreaseHp(int value, CreatureRef attacker)
{
	statInfo->set_hp(statInfo->hp() - value);
	cout << "Player Hp : " << statInfo->hp() << endl;
	if (statInfo->hp() < 0)
	{
		statInfo->set_hp(0);
		owner.lock()->Die(attacker);
	}
	SendStatPacket();
}

void StatComponent::IncreaseMp(int value)
{
	statInfo->set_mp(statInfo->mp() + value);
	if (statInfo->mp() > defaultStat->mp)
	{
		statInfo->set_mp(defaultStat->mp);
	}
	SendStatPacket();
}

void StatComponent::DecreaseMp(int value)
{
	statInfo->set_mp(statInfo->mp() - value);
	cout << "Player Mp : " << statInfo->mp() << endl;
	if (statInfo->mp() < 0)
	{
		statInfo->set_mp(0);
	}
	SendStatPacket();
}

void StatComponent::IncreaseAtk(int value)
{
	statInfo->set_atk(statInfo->atk() + value);
	SendStatPacket();
}

void StatComponent::DecreaseAtk(int value)
{
	statInfo->set_atk(statInfo->atk() - value);
	SendStatPacket();
}

void StatComponent::IncreaseDfs(int value)
{
	statInfo->set_dfs(statInfo->dfs() + value);
	SendStatPacket();
}

void StatComponent::DecreaseDfs(int value)
{
	statInfo->set_dfs(statInfo->dfs() - value);
	SendStatPacket();
}

void StatComponent::SendStatPacket()
{
	Protocol::S_STATCHANGED pkt;

	pkt.set_object_id(owner.lock()->GetObjectInfo()->object_id());
	Protocol::StatInfo* stat = pkt.mutable_statinfo();
	stat->CopyFrom(*statInfo);

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->GetRoom()->Broadcast(sendBuffer);
}

