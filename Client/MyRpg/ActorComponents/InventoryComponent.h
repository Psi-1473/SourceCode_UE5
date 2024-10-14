// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:

	void AddItem(const Protocol::ItemUpdateInfo& itemInfo, bool ByUnEquip);
	void RemoveItem(TArray<uint64> DbId);

	void EquipItem(uint64 DbId, uint64 PrevItemDbId, int32 PrevItemSlotIndex);
	void UnEquipItem(int32 SlotIndex);
	void SwapItem(Protocol::EItemType ItemType, int32 SlotIndex1, int32 SlotIndex2, TWeakObjectPtr<class UInventoryWidget> InvenWidget);

public:
	/*
		Get / Set Functions
	*/
	TObjectPtr<class UItem> GetItem(uint64 DbId) { return AllItems[DbId]; }
	const TArray<TObjectPtr<UItem>>& GetEquippedItems() { return EquippedItems; }
	const TArray<TObjectPtr<UItem>>& GetEquipments() { return Equipments; }
	const TArray<TObjectPtr<UItem>>& GetConsumables() { return Consumables; }
	int32 GetGold() { return Gold; }

	void SetGold(int32 Value) { Gold = Value; }
	void SetOwnerPlayer(TWeakObjectPtr<class AMyPlayer> Player) { OwnerPlayer = Player; }
private:
	void SetToInventory(TArray<TObjectPtr<UItem>>& Container, TObjectPtr<class UItem> Item, int32 SlotIndex);
	void EquipItem(TObjectPtr<UItem> EquipItem, int32 PrevItemDbId, int32 PrevItemSlotIndex);
	void RemoveFromInventory(TObjectPtr<UItem> Item);
	void RemoveFromEquipped(TObjectPtr<UItem> Item);
	void RemoveFromQuickSlot(TObjectPtr<UItem> Item);

	int32 GetEquipSlotIndex(TObjectPtr<class UItem> Item);

private:
	TObjectPtr<class UItem> CreateNewItem(Protocol::EItemType ItemType, int32 TemplateId);

	TObjectPtr<class UWeapon> CreateNewWeapon(int32 TemplateId);
	TObjectPtr<class UArmor> CreateNewArmor(int32 TemplateId);
	TObjectPtr<class UConsumable> CreateNewConsumable(int32 TemplateId);
	
	/* 인벤토리에 해당 아이템이 있나 확인한 후, 없으면 생성하는 함수*/
	TObjectPtr<UItem> GetOrCreateItem(uint64 DbId, Protocol::EItemType ItemType, int32 TemplateId);
	TArray<TObjectPtr<class UItem>>& GetInven(Protocol::EItemType ItemType);
private:
	void SendItemSwapPacket(TObjectPtr<UItem> Item1, TObjectPtr<UItem> Item2, int32 SlotIndex1, int32 SlotIndex2);

public:
	UPROPERTY()
	TArray<TObjectPtr<class UItem>> EquippedItems;
	// Helmet ~ Gloves 0 ~ 4, Weapon 5

	UPROPERTY()
	TMap<uint64, TObjectPtr<UItem>> AllItems;

	UPROPERTY()
	TArray<TObjectPtr<class UItem>> Equipments;

	UPROPERTY()
	TArray<TObjectPtr<class UItem>> Consumables;

	UPROPERTY()
	TWeakObjectPtr<class AMyPlayer> OwnerPlayer;

	int32 Gold = 0;
	
};
