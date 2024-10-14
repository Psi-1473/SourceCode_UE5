// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "RpgPlayer.h"
#include "MyGameInstance.h"

void UItem::Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type)
{
	DbId = DataId;
	Count = Cnt;
	SlotIndex = SlotIdx;
	SlotState = State;
	ItemType = Type;
	QuickSlotKey = Protocol::QUICK_NONE;
}

void UItem::UseItem(TObjectPtr<class UMyGameInstance> GInstance)
{
}

bool UItem::GetUsableItem(TObjectPtr<class ARpgPlayer> Player)
{
	if (ItemType == Protocol::ITEM_CONSUMABLE)
	{
		return true;
	}

	if (ItemType == Protocol::ITEM_ARMOR)
	{
		auto EquipData = static_cast<FArmorData*>(ItemData);
		
		if (EquipData->Class != Player->GetObjectInfo()->objtype())
		{
			return false;
		}
	}

	if (ItemType == Protocol::ITEM_WEAPON)
	{
		auto EquipData = static_cast<FWeaponData*>(ItemData);

		if (EquipData->Class != Player->GetObjectInfo()->objtype())
		{
			return false;
		}
	}

	return true;
}


