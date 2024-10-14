#include "pch.h"
#include "MonsterStatComponent.h"
#include "Creature.h"
#include "DBStruct.h"
#include "DataManager.h"
#include "Room.h"

void MonsterStatComponent::SetDefaultStat(int32 levelOrId)
{
	defaultStat = GDataManager->GetMonsterStat(levelOrId);
}

void MonsterStatComponent::Init(CreatureRef creature, int32 level)
{
	StatComponent::Init(creature, level);
	statInfo->set_exp(defaultStat->exp);
}
