#include "pch.h"
#include "Consumable.h"
#include "DataManager.h"
#include "StatComponent.h"
#include "Player.h"

Consumable::Consumable(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state, int newSlotIndex)
: Item(dId, templateId, itemCount, state, newSlotIndex)
{
	itemType = Protocol::ITEM_CONSUMABLE;
	itemInfo = GDataManager->GetConsumableData(templateId);
}

void Consumable::UseItem(PlayerRef player)
{
	// 1. 아이템 데이터 가져오기
	
	shared_ptr<ConsumableInfo> info = PTR_CAST(ConsumableInfo, itemInfo);
	Protocol::EHealType healType = info->healType;
	int healValue = itemInfo->power;

	switch (healType)
	{
		case Protocol::HEAL_HP:
			player->GetStatComponent()->IncreaseHp(healValue);
			break;
		case Protocol::HEAL_MP:
			player->GetStatComponent()->IncreaseMp(healValue);
			break;
		default:
			break;
	}
}
