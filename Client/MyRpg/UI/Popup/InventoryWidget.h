// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "Protocol.pb.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UInventoryWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UInventoryComponent> NewInventory, TWeakObjectPtr<class UShopWidget> NewShopWidget = nullptr);
	void AddChildWidget(TObjectPtr<UUserWidget> Widget);

	UFUNCTION()
	void CloseThisUI();

	UFUNCTION()
	void SetEquipment();

	UFUNCTION()
	void SetConsumable();

	virtual void Refresh() override;
public:
	bool GetIsShopMode();
	TWeakObjectPtr<class UShopWidget> GetShopWidget() { return ShopWidget; }
private:
	void CreateSlots();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InvenSlotClass;
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow;
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow_1;
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow_2;
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow_3;
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow_4;
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> SlotRow_5;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCanvasPanel> Canvas;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Equip;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Consumable;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Misc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Gold;

private:
	UPROPERTY()
	TWeakObjectPtr<class UShopWidget> ShopWidget;

	UPROPERTY()
	TArray<TObjectPtr<class UInvenSlotWidget>> InvenSlots;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;
private:
	bool bShopMode = false;
	Protocol::EItemType ItemType;
	int32 SLOT_COUNT = 6;
	int32 SLOT_ROW_COUNT = 6;
};
