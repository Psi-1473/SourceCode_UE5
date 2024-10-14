// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UEquipmentWidget : public UPopupWidget
{
	GENERATED_BODY()


public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);
	virtual void Refresh() override;

	UFUNCTION()
	void CloseThisUI();

private:
	FText GetClassName(TWeakObjectPtr<class AMyPlayer> Player);

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UEquipSlotWidget> Helmet;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEquipSlotWidget> Top;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEquipSlotWidget> Bottom;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEquipSlotWidget> Boots;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEquipSlotWidget> Gloves;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEquipSlotWidget> Weapon;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Class;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Level;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Hp;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Mp;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Atk;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Dfs;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;
private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
	
};
