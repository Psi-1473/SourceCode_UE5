// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/InventoryComponent.h"
#include "Item/Weapon.h"
#include "Item/Armor.h"
#include "Item/Consumable.h"
#include "Popup/InventoryWidget.h"
#include "MyPlayer.h"
#include "MyRpg.h"
#include "MyGameInstance.h"
#include "UIManager.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Equipments.Init(nullptr, 36);
	Consumables.Init(nullptr, 36);
	EquippedItems.Init(nullptr, 6);

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::AddItem(const Protocol::ItemUpdateInfo& itemInfo, bool ByUnEquip)
{
	uint64 DbId = itemInfo.dbid();
	uint32 TemplateId = itemInfo.templateid();
	Protocol::EItemType ItemType = itemInfo.itemtype();
	int32 SlotIndex = itemInfo.slotnumber();
	int32 Count = itemInfo.number();


	TObjectPtr<UItem> NewItem = GetOrCreateItem(DbId, ItemType, TemplateId);
	NewItem->Init(TemplateId, DbId, Count, SlotIndex, Protocol::SLOT_INVENTORY, ItemType);
	SetToInventory(GetInven(ItemType), NewItem, SlotIndex);
	AllItems.Add(DbId, NewItem);
	
	auto GInstance = Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance());
	if (ByUnEquip)
	{
		int32 EquipSlot = GetEquipSlotIndex(NewItem);
		EquippedItems[EquipSlot] = nullptr;
			
	}

	GInstance->GetUIMgr()->RefreshUI(TEXT("Equipment"));
	GInstance->GetUIMgr()->RefreshUI(TEXT("Shop"));
	GInstance->GetUIMgr()->RefreshUI(TEXT("Inventory"));
}

void UInventoryComponent::RemoveItem(TArray<uint64> DbIds)
{
	for(int i = 0; i < DbIds.Num(); i++)
	{
		TObjectPtr<UItem> Item = AllItems[DbIds[i]];

		RemoveFromInventory(Item);
		RemoveFromEquipped(Item);
		RemoveFromQuickSlot(Item);

		AllItems.Remove(DbIds[i]);
	}
	auto GInstance = Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance());
	GInstance->GetUIMgr()->RefreshUI(TEXT("Shop"));
	GInstance->GetUIMgr()->RefreshUI(TEXT("Inventory"));
}

void UInventoryComponent::EquipItem(uint64 DbId, uint64 PrevItemDbId, int32 PrevItemSlotIndex)
{
	// 1. 해당하는 아이템 찾기
	TObjectPtr<UItem> ItemToEquip = AllItems[DbId];
	if (ItemToEquip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL ITEM"));
	}

	// 3. 아이템 장착
	EquipItem(ItemToEquip, PrevItemDbId, PrevItemSlotIndex);
	
	// ======= Temp : 위젯 ========
	Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance())->GetUIMgr()->RefreshUI(TEXT("Equipment"));
	Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance())->GetUIMgr()->RefreshUI(TEXT("Inventory"));
}

void UInventoryComponent::UnEquipItem(int32 SlotIndex)
{
	// 1. ItemToChange == nullptr -> ㅁㅁ
}

void UInventoryComponent::SwapItem(Protocol::EItemType ItemType, int32 SlotIndex1, int32 SlotIndex2, TWeakObjectPtr<UInventoryWidget> InvenWidget)
{
	TArray<TObjectPtr<UItem>>& Inven = GetInven(ItemType);
	
	TObjectPtr<UItem> Item1 = Inven[SlotIndex1];
	TObjectPtr<UItem> Item2 = Inven[SlotIndex2];

	Inven[SlotIndex1] = Item2;
	Inven[SlotIndex2] = Item1;

	SendItemSwapPacket(Item1, Item2, SlotIndex1, SlotIndex2);

	// todo 2) 인벤토리 위젯 업데이트
	InvenWidget->Refresh();

}


void UInventoryComponent::SetToInventory(TArray<TObjectPtr<UItem>>& Container, TObjectPtr<UItem> Item, int32 SlotIndex)
{
	if (SlotIndex < 0)
	{
		return;
	}
	Container[SlotIndex] = Item;
	Item->SetSlotIndex(SlotIndex);
	Item->SetSlotState(Protocol::SLOT_INVENTORY);
}
void UInventoryComponent::EquipItem(TObjectPtr<class UItem> ItemToEquip, int32 PrevItemDbId, int32 PrevItemSlotIndex)
{
	TObjectPtr<UItem> PrevItem = nullptr;
	if (ItemToEquip->GetSlotIndex() >= 0)
	{
		Equipments[ItemToEquip->GetSlotIndex()] = nullptr;
	}
	if (PrevItemSlotIndex >= 0)
	{
		PrevItem = AllItems[PrevItemDbId];
	}

	int32 EquipSlotIndex = GetEquipSlotIndex(ItemToEquip);

	EquippedItems[EquipSlotIndex] = ItemToEquip;
	ItemToEquip->SetSlotIndex(-1);
	ItemToEquip->SetSlotState(Protocol::SLOT_EQUIPPED);

	if (PrevItem != nullptr)
	{
		SetToInventory(Equipments, PrevItem, PrevItemSlotIndex);
	}
}
void UInventoryComponent::RemoveFromInventory(TObjectPtr<UItem> Item)
{
	int32 Index = -1;
	switch (Item->GetItemType())
	{
		case Protocol::ITEM_CONSUMABLE:
			Index = Consumables.Find(Item);
			if (Index >= 0)
			{
				Consumables[Index] = nullptr;
			}
			break;
		default:
			Index = Equipments.Find(Item);
			if (Index >= 0)
			{
				Equipments[Index] = nullptr;
			}
			break;
	}
}
void UInventoryComponent::RemoveFromEquipped(TObjectPtr<UItem> Item)
{
	if (Item->GetItemType() != Protocol::ITEM_ARMOR && Item->GetItemType() != Protocol::ITEM_WEAPON)
	{
		return;
	}
	int32 EquipSlotIndex = GetEquipSlotIndex(Item);

	if (EquippedItems[EquipSlotIndex] == Item)
	{
		EquippedItems[EquipSlotIndex] = nullptr;
	}
}
void UInventoryComponent::RemoveFromQuickSlot(TObjectPtr<class UItem> Item)
{
	Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance())->SetQuickSlotNull(Item->GetQuickSlotKey());
	Item->SetQuickSlot(Protocol::QUICK_NONE);
}
int32 UInventoryComponent::GetEquipSlotIndex(TObjectPtr<UItem> Item)
{
	if (Cast<UWeapon>(Item) != nullptr)
	{
		return 5;
	}
	else
	{
		//return (int32)(Cast<FArmorData>(Item->GetItemData())->ArmorType);
		return (int32)Cast<UArmor>(Item)->GetArmorData()->ArmorType;
	}
}
TObjectPtr<class UItem> UInventoryComponent::CreateNewItem(Protocol::EItemType ItemType, int32 TemplateId)
{
	TObjectPtr<UItem> NewItem;

	switch (ItemType)
	{
		case Protocol::ITEM_WEAPON:
			NewItem = CreateNewWeapon(TemplateId);
			break;
		case Protocol::ITEM_ARMOR:
			NewItem = CreateNewArmor(TemplateId);
			break;
		case Protocol::ITEM_CONSUMABLE:
			NewItem = CreateNewConsumable(TemplateId);
			break;

	}

	return NewItem;
}
TObjectPtr<class UWeapon> UInventoryComponent::CreateNewWeapon(int32 TemplateId)
{
	TObjectPtr<class UWeapon> NewWeapon = NewObject<UWeapon>();
	return NewWeapon;
}
TObjectPtr<class UArmor> UInventoryComponent::CreateNewArmor(int32 TemplateId)
{
	TObjectPtr<class UArmor> NewArmor = NewObject<UArmor>();
	return NewArmor;
}
TObjectPtr<class UConsumable> UInventoryComponent::CreateNewConsumable(int32 TemplateId)
{
	TObjectPtr<class UConsumable> NewConsumable = NewObject<UConsumable>();
	return NewConsumable;
}
TObjectPtr<UItem> UInventoryComponent::GetOrCreateItem(uint64 DbId, Protocol::EItemType ItemType, int32 TemplateId)
{
	if (AllItems.Contains(DbId))
	{
		return AllItems[DbId];
	}
	else
	{
		return CreateNewItem(ItemType, TemplateId);
	}
}
TArray<TObjectPtr<class UItem>>& UInventoryComponent::GetInven(Protocol::EItemType ItemType)
{
	switch (ItemType)
	{
	case Protocol::ITEM_CONSUMABLE:
		return Consumables;
		break;
	default:
		return Equipments;
		break;
	}
}
void UInventoryComponent::SendItemSwapPacket(TObjectPtr<UItem> Item1, TObjectPtr<UItem> Item2, int32 SlotIndex1, int32 SlotIndex2)
{
	Protocol::C_ITEMSWAP Pkt;
	{
		Pkt.set_playerid(OwnerPlayer->GetObjectInfo()->object_id());
		Pkt.set_item1_dbid(-1);
		Pkt.set_item2_dbid(-1);
		Pkt.set_item1_slot(-1);
		Pkt.set_item2_slot(-1);
	}

	if (Item1 != nullptr)
	{
		Item1->SetSlotIndex(SlotIndex2);
		Pkt.set_item1_dbid(Item1->GetDbId());
		Pkt.set_item1_slot(SlotIndex2);
	}
	if (Item2 != nullptr)
	{
		Item2->SetSlotIndex(SlotIndex1);
		Pkt.set_item2_dbid(Item2->GetDbId());
		Pkt.set_item2_slot(SlotIndex1);
	}

	// todo 1) 서버에 아이템 바뀜 패킷 보내기
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	Cast<UMyGameInstance>(OwnerPlayer->GetWorld()->GetGameInstance())->SendPacket(SendBuffer);

}



