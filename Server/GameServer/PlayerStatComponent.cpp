#include "pch.h"
#include "Player.h"
#include "Inventory.h"
#include "PlayerStatComponent.h"
#include "SkillComponent.h"
#include "Creature.h"
#include "DBStruct.h"
#include "DataManager.h"
#include "Room.h"

void PlayerStatComponent::SetDefaultStat(int32 levelOrId)
{
	defaultStat = GDataManager->GetCharacterStat(owner.lock()->GetObjectInfo()->objtype(), levelOrId);
}

void PlayerStatComponent::Init(CreatureRef creature, int32 level)
{
	StatComponent::Init(creature, level);
	if(level == 1)
		statInfo->set_exp(0);

	PTR_CAST(Player, owner.lock())->GetInventory()->ApplyItemStat();
}

void PlayerStatComponent::IncreaseExp(int value)
{
	statInfo->set_exp(statInfo->exp() + value);
	cout << "exp : " << statInfo->exp() << " : " << defaultStat->exp << endl;
	if (statInfo->exp() >= defaultStat->exp)
	{
		int leftExp = statInfo->exp() - defaultStat->exp;
		// 1. Level Up 패킷 보내기
		LevelUp(owner.lock(), statInfo->level() + 1);
		
		statInfo->set_exp(leftExp);
	}
	SendStatPacket();
}

void PlayerStatComponent::DecreaseExp(int value)
{
	statInfo->set_exp(statInfo->exp() - value);
	if (statInfo->exp() < 0)
	{
		statInfo->set_exp(0);
	}
	SendStatPacket();
}

void PlayerStatComponent::LevelUp(CreatureRef creature, int32 level)
{
	Init(creature, level);
	PTR_CAST(Player, owner.lock())->GetSkillComponent()->LevelUp();

	// DB 레벨업
	Protocol::S_LEVEL_UP pkt;
	pkt.set_object_id(creature->GetObjectInfo()->object_id());
	pkt.set_level(level);
	
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	creature->GetRoom()->Broadcast(sendBuffer);

	GDBManager->DoAsync(&DBManager::LevelUp, PTR_CAST(Player, owner.lock()), level);
}


