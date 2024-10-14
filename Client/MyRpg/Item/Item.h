// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataStruct.h"
#include "Protocol.pb.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UItem : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type);
	
	virtual void UseItem(TObjectPtr<class UMyGameInstance> GInstance);

	bool GetUsableItem(TObjectPtr<class ARpgPlayer> Player);
public:
	void SetQuickSlot(const Protocol::EQuickSlot Key) { QuickSlotKey = Key; }
	void SetCount(int32 NewCount) { Count = NewCount; }
	void SetCountToBeSold(int32 NewCount) { CountToBeSold = NewCount; }
	void SetSlotIndex(int32 Index) { SlotIndex = Index; }
	void SetSlotState(Protocol::ESlotState State) { SlotState = State; }
	void SetToBeSold(bool Value) { bToBeSold = Value; }

	uint64 GetDbId() { return DbId; }
	int32 GetCount() { return Count; }
	int32 GetCountToBeSold() { return CountToBeSold; }
	int32 GetSlotIndex() { return SlotIndex; }
	Protocol::EQuickSlot GetQuickSlotKey() { return QuickSlotKey; }
	bool GetToBeSold() { return bToBeSold; }
	const Protocol::ESlotState& GetSlotState() { return SlotState; }
	const Protocol::EItemType& GetItemType () { return ItemType; }
	virtual const Protocol::ECharacterType GetClass() { return Protocol::NONE; }
	struct FRichImageRow* GetItemImage() const { return ItemImage; }

	const FItemData* GetItemData() const { return ItemData; }
protected:
	FItemData* ItemData;
	struct FRichImageRow* ItemImage;

	uint64 DbId;
	int32 Count;
	int32 CountToBeSold;
	int32 SlotIndex;

	Protocol::EQuickSlot QuickSlotKey;
	Protocol::ESlotState SlotState;
	Protocol::EItemType ItemType;
	
	bool bToBeSold = false;
};
