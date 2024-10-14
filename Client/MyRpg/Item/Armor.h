// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Armor.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UArmor : public UItem
{
	GENERATED_BODY()

public:
	virtual void Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type) override;
	virtual void UseItem(TObjectPtr<class UMyGameInstance> GInstance) override;
	void UnEquip(TObjectPtr<class UMyGameInstance> GInstance);
	virtual const Protocol::ECharacterType GetClass() override;
	
public:
	const FArmorData* GetArmorData() const { return static_cast<FArmorData*>(ItemData); }

};
