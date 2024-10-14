#include "pch.h"
#include "Weapon.h"
#include "DataManager.h"


Weapon::Weapon(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state, int newSlotIndex)
: Equipment(dId, templateId, itemCount, state, newSlotIndex)
{	
	itemType = Protocol::ITEM_WEAPON;
	itemInfo = GDataManager->GetWeaponData(templateId);

}
