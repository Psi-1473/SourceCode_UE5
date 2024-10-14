// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Consumable.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UConsumable : public UItem
{
	GENERATED_BODY()

public:
	virtual void Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type) override;
	virtual void UseItem(TObjectPtr<class UMyGameInstance>) override;
	virtual const Protocol::ECharacterType GetClass() override;

};
