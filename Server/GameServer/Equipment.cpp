#include "pch.h"
#include "Equipment.h"

Equipment::Equipment(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state, int newSlotIndex)
: Item(dId, templateId, itemCount, state, newSlotIndex)
{
	
}

void Equipment::UseItem(PlayerRef player)
{
}
