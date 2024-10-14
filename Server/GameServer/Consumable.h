#pragma once
#include "Item.h"

class Consumable : public Item
{
public: 
	Consumable() = delete;
	Consumable(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state = Protocol::SLOT_INVENTORY, int newSlotIndex = -1);

	virtual void UseItem(PlayerRef player) override;
};

