#pragma once
#include "DBStruct.h"

class Item
{
public:
	Item() = delete;
	Item(uint64 dId, uint32 templateId, uint32 itemCount, Protocol::ESlotState state = Protocol::SLOT_INVENTORY, int newSlotIndex = -1);

	virtual void UseItem(PlayerRef player) {};


public:
	/* Get / Set Functions */
	uint64 GetDbId() { return dataId; }
	Protocol::EItemType GetItemType() { return itemType; }
	shared_ptr<ItemInfo> GetItemInfo() { return itemInfo; }
	uint32 GetCount() { return count; }
	int32 GetSlotIndex() { return slotIndex; }
	Protocol::EQuickSlot GetQuickSlot() { return quickType; }


	void SetSlotState(Protocol::ESlotState state) { slotState = state; }
	void SetCount(uint32 newCount) { count = newCount; }
	void SetCount(uint32 newCount, PlayerRef player);
	void SetSlotIndex(uint32 value) { slotIndex = value; }
	void SetQuickType(Protocol::EQuickSlot type) { quickType = type; }

protected:
	uint64 dataId;
	Protocol::EItemType itemType;
	shared_ptr<ItemInfo> itemInfo;
	Protocol::ESlotState slotState;
	uint32 count;
	int32 slotIndex;
	Protocol::EQuickSlot quickType;
};

