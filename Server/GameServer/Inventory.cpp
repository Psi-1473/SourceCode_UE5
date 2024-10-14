#include "pch.h"
#include "Inventory.h"
#include "DBStruct.h"
#include "Armor.h"
#include "Player.h"
#include "GameSession.h"
#include "Room.h"
#include "StatComponent.h"
#include "QuickSlotComponent.h"

#include "QuestComponent.h"
Inventory::Inventory()
{
	for(int i = 0; i < MAX_SLOT_COUNT; i++)
		emptySlot_Equipment.insert(i);
	for (int i = 0; i < MAX_SLOT_COUNT; i++)
		emptySlot_Consumable.insert(i);
}

void Inventory::Init(PlayerRef newOwner)
{
	// 1. 오너 설정
	owner = newOwner;

}

void Inventory::AddItemToInventory(ItemRef item, int slotIndex, bool isOnlyUneuqipResult, bool bSendToClient)
{	
	/*
		Stack Count를 채우는 것만으로 작업이 끝나지 않는, 어쨌든 새로운 고유 아이템을 만들어야 할 때 사용
	*/

	// isOnlyUneuqipResult : 장착하려는 장비도 없고 "오직" 아이템 장착 해제만으로 인해 일어나는가?
	// bSendToClient : 바뀐 사실을 클라이언트한테 패킷으로 보낼 것인가?

	// 1. item의 slotIndex 설정
	item->SetSlotIndex(slotIndex);
	PopSlotIndex(item, slotIndex);
	// 2. 모든 아이템 목록에 해당 아이템 추가
	allItems[item->GetDbId()] = item;

	if (item->GetQuickSlot() != Protocol::QUICK_NONE)
	{
		owner.lock()->GetQuickSlotComponent()->SetElement(item->GetQuickSlot(),Protocol::QUICK_ITEM, item->GetDbId());
	}
	
	// 3. 클라이언트한테 생성한 아이템 정보와 슬롯 넘겨주기

	if(bSendToClient)
	{
		Protocol::S_ADDITEM pkt;

		Protocol::ItemUpdateInfo* itemInfo = pkt.mutable_item();
		itemInfo->set_dbid(item->GetDbId());
		itemInfo->set_slotnumber(slotIndex);
		itemInfo->set_number(item->GetCount()); // number는 바뀔 수 있음
		itemInfo->set_templateid(item->GetItemInfo()->templateId);
		itemInfo->set_itemtype(item->GetItemType());
		pkt.set_byunequip(isOnlyUneuqipResult);
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
	}

	if (item->GetItemType() == Protocol::ITEM_CONSUMABLE)
	{
		cout << " (Inventory) -- Check Item Quest" << endl;
		owner.lock()->GetQuestComponent()->TargetUp(item->GetItemInfo()->templateId, Protocol::TARGET_CONSUMABLE);
	}
	
}

void Inventory::RemoveItem(uint64 dbId)
{
	ItemRef item = allItems[dbId];

	if (item->GetItemType() == Protocol::ITEM_CONSUMABLE)
	{
		emptySlot_Consumable.insert(item->GetSlotIndex());
		owner.lock()->GetQuestComponent()->TargetUp(item->GetItemInfo()->templateId, Protocol::TARGET_CONSUMABLE);
	}
	else
	{
		emptySlot_Equipment.insert(item->GetSlotIndex());

		int equipSlotIndex = GetEquippedSlotIndex(dbId);
		if (equippedItems[equipSlotIndex] == item)
		{
			equippedItems[equipSlotIndex] = nullptr;
		}
	}

	
	allItems.erase(dbId);
}


void Inventory::EquipItem(uint64 dbId)
{
	// 장착 아이템의 장착
	int32 prevSlotIndex = allItems[dbId]->GetSlotIndex();
	if (equippedItems[GetEquippedSlotIndex(dbId)] != nullptr)
	{
		uint64 prevItemId = equippedItems[GetEquippedSlotIndex(dbId)]->GetDbId();
		UpdatePlayerStat(prevItemId, false);
	}
	equippedItems[GetEquippedSlotIndex(dbId)] = PTR_CAST(Equipment, allItems[dbId]);

	if(prevSlotIndex >= 0)
		emptySlot_Equipment.insert(prevSlotIndex);

	allItems[dbId]->SetSlotIndex(-1);
	allItems[dbId]->SetSlotState(Protocol::SLOT_EQUIPPED);

	// 2. 캐릭터 스탯 업데이트
	UpdatePlayerStat(dbId, true);
}

void Inventory::UnEquipItem(ItemRef unequipItem)
{
	int equippedSlot = GetEquippedSlotIndex(unequipItem->GetDbId());
	equippedItems[equippedSlot] = nullptr;

	// 캐릭터 스탯 업데이트
	UpdatePlayerStat(unequipItem->GetDbId(), false);
}


void Inventory::SwapItem(const Protocol::C_ITEMSWAP& pkt)
{
	if(pkt.item1_slot() >= 0)
	{
		ItemRef item1 = allItems[pkt.item1_dbid()];
		if(pkt.item2_slot() < 0)
		{
			EmptySlot(item1);
		}
		item1->SetSlotIndex(pkt.item1_slot());
		FillSlot(item1);
	}

	if (pkt.item2_slot() >= 0)
	{
		ItemRef item2 = allItems[pkt.item2_dbid()];
		if (pkt.item1_slot() < 0)
		{
			EmptySlot(item2);
		}
		item2->SetSlotIndex(pkt.item2_slot());
		FillSlot(item2);
	}
}

void Inventory::FillSlot(ItemRef item)
{
	switch (item->GetItemType())
	{
	case Protocol::ITEM_CONSUMABLE:
		emptySlot_Consumable.erase(item->GetSlotIndex());
		break;
	default:
		emptySlot_Equipment.erase(item->GetSlotIndex());
		break;
	}
}

void Inventory::EmptySlot(ItemRef item)
{
	switch (item->GetItemType())
	{
		case Protocol::ITEM_CONSUMABLE:
			emptySlot_Consumable.insert(item->GetSlotIndex());
			break;
		default:
			emptySlot_Equipment.insert(item->GetSlotIndex());
			break;
	}
}

int32 Inventory::GetSpecificItemCount(Protocol::EItemType itemType, int32 templateId)
{
	int count = 0;
	for (const auto& item : allItems)
	{
		if(item.second->GetItemType() != itemType)
			continue;

		if(item.second->GetItemInfo()->templateId == templateId)
			count += item.second->GetCount();
		cout << "count : " << count << endl;
	}

	return count;
}

void Inventory::ApplyItemStat()
{
	for (const auto& item : equippedItems)
	{
		if(item.second == nullptr)
			continue;

		UpdatePlayerStat(item.second->GetDbId(), true);
	}
}

void Inventory::SetGold(int32 value, bool bSendPacket)
{
	gold = value;

	if (bSendPacket)
	{
		Protocol::S_SETGOLD pkt;
		pkt.set_gold(gold);
		pkt.set_object_id(owner.lock()->GetDbId());

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
	}
}

int Inventory::FindSlotIndex(Protocol::EItemType itemType)
{
	uint32 slotIndex = -1;

	switch (itemType)
	{
	case Protocol::ITEM_WEAPON:
		if (emptySlot_Equipment.size() == 0)
			break;
		slotIndex = *emptySlot_Equipment.begin();
		emptySlot_Equipment.erase(slotIndex);
		break;
	case Protocol::ITEM_ARMOR:
		if (emptySlot_Equipment.size() == 0)
			break;
		slotIndex = *emptySlot_Equipment.begin();
		emptySlot_Equipment.erase(slotIndex);
		break;
	case Protocol::ITEM_CONSUMABLE:
		if (emptySlot_Consumable.size() == 0)
			break;
		slotIndex = *emptySlot_Consumable.begin();
		emptySlot_Consumable.erase(slotIndex);
		break;
	}

	return slotIndex;
}

bool Inventory::GetStackableDbId(Protocol::EItemType itemType, int32 templateId, uint64& dbId, int32 count)
{
	// 스택 가능한 아이템인지, 해당 인벤토리에 스택할 수 있는 같은 종류의 아이템이 있는가 확인
	// 불가능 : return false
	// 가능 : return true, dbId 값 설정
	if (itemType != Protocol::ITEM_CONSUMABLE) // 추후에 기타 아이템도 추가
	{
		return false;
	}

	for (const auto& value : allItems)
	{
		ItemRef item = value.second;
		if (item->GetItemType() != itemType)
			continue;
		if(item->GetItemInfo()->templateId != templateId)
			continue;
		if(item->GetCount() + count > MAX_ITEM_COUNT)
			continue;

		dbId = value.first;
		return true;
	}
	return false;
}


int Inventory::GetEquippedSlotIndex(uint64 dbId)
{
	if(allItems[dbId]->GetItemType() == Protocol::ITEM_WEAPON)
		return 5;
	else
		return (int)PTR_CAST(ArmorInfo, allItems[dbId]->GetItemInfo())->armorType;
}

void Inventory::UpdatePlayerStat(uint64 dbId, bool bIncrease)
{
	ItemRef item = allItems[dbId];
	shared_ptr<StatComponent> statComp = owner.lock()->GetStatComponent();
	if (item->GetItemType() == Protocol::ITEM_WEAPON)
	{
		if(bIncrease)
			statComp->IncreaseAtk(item->GetItemInfo()->power);
		else
			statComp->DecreaseAtk(item->GetItemInfo()->power);
	}
	else
	{
		if (bIncrease)
			statComp->IncreaseDfs(item->GetItemInfo()->power);
		else
			statComp->DecreaseDfs(item->GetItemInfo()->power);
	}
}

void Inventory::PopSlotIndex(ItemRef item, int slotIndex)
{
	if (item->GetItemType() == Protocol::ITEM_CONSUMABLE)
	{
		emptySlot_Consumable.erase(slotIndex);
	}
	else
	{
		emptySlot_Equipment.erase(slotIndex);
	}
}
