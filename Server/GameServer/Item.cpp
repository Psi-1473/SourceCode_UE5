#include "pch.h"
#include "Item.h"
#include "DataManager.h"

#include "Player.h"
#include "QuestComponent.h"



Item::Item(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state, int newSlotIndex)
{
	dataId = dId;
	slotState = state;
	count = itemCount;
	slotIndex = newSlotIndex;
	quickType = Protocol::QUICK_NONE;
}

void Item::SetCount(uint32 newCount, PlayerRef player)
{
	count = newCount;
	player->GetQuestComponent()->TargetUp(GetItemInfo()->templateId, Protocol::TARGET_CONSUMABLE);
}


