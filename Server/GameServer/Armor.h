#pragma once
#include "Equipment.h"

class Armor : public Equipment
{
public:
	Armor() = delete;
	Armor(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state = Protocol::SLOT_INVENTORY, int newSlotIndex = -1);

private:

};

