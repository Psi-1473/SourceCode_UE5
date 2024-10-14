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
	// 1. ���� ����
	owner = newOwner;

}

void Inventory::AddItemToInventory(ItemRef item, int slotIndex, bool isOnlyUneuqipResult, bool bSendToClient)
{	
	/*
		Stack Count�� ä��� �͸����� �۾��� ������ �ʴ�, ��·�� ���ο� ���� �������� ������ �� �� ���
	*/

	// isOnlyUneuqipResult : �����Ϸ��� ��� ���� "����" ������ ���� ���������� ���� �Ͼ�°�?
	// bSendToClient : �ٲ� ����� Ŭ���̾�Ʈ���� ��Ŷ���� ���� ���ΰ�?

	// 1. item�� slotIndex ����
	item->SetSlotIndex(slotIndex);
	PopSlotIndex(item, slotIndex);
	// 2. ��� ������ ��Ͽ� �ش� ������ �߰�
	allItems[item->GetDbId()] = item;

	if (item->GetQuickSlot() != Protocol::QUICK_NONE)
	{
		owner.lock()->GetQuickSlotComponent()->SetElement(item->GetQuickSlot(),Protocol::QUICK_ITEM, item->GetDbId());
	}
	
	// 3. Ŭ���̾�Ʈ���� ������ ������ ������ ���� �Ѱ��ֱ�

	if(bSendToClient)
	{
		Protocol::S_ADDITEM pkt;

		Protocol::ItemUpdateInfo* itemInfo = pkt.mutable_item();
		itemInfo->set_dbid(item->GetDbId());
		itemInfo->set_slotnumber(slotIndex);
		itemInfo->set_number(item->GetCount()); // number�� �ٲ� �� ����
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
	// ���� �������� ����
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

	// 2. ĳ���� ���� ������Ʈ
	UpdatePlayerStat(dbId, true);
}

void Inventory::UnEquipItem(ItemRef unequipItem)
{
	int equippedSlot = GetEquippedSlotIndex(unequipItem->GetDbId());
	equippedItems[equippedSlot] = nullptr;

	// ĳ���� ���� ������Ʈ
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
	// ���� ������ ����������, �ش� �κ��丮�� ������ �� �ִ� ���� ������ �������� �ִ°� Ȯ��
	// �Ұ��� : return false
	// ���� : return true, dbId �� ����
	if (itemType != Protocol::ITEM_CONSUMABLE) // ���Ŀ� ��Ÿ �����۵� �߰�
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
