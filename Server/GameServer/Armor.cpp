#include "pch.h"
#include "Armor.h"
#include "DataManager.h"

Armor::Armor(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state, int newSlotIndex)
:Equipment(dId, templateId, itemCount, state, newSlotIndex)
{
	itemType = Protocol::ITEM_ARMOR;
	itemInfo = GDataManager->GetArmorData(templateId);
}
