#pragma once
#include "Equipment.h"

class Weapon : public Equipment
{
public:
	Weapon() = delete;
	Weapon(uint64 dId, uint32 templatedId, uint32 itemCount, Protocol::ESlotState state = Protocol::SLOT_INVENTORY, int newSlotIndex = -1);
private:
};

