#pragma once
#include "Item.h"

class Equipment : public Item
{
public:
	Equipment() = delete;
	Equipment(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state = Protocol::SLOT_INVENTORY, int newSlotIndex = -1);
	virtual void UseItem(PlayerRef player) override;
protected:
};

