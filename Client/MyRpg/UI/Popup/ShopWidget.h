// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */

enum ShopMode
{
	SHOP_BUY,
	SHOP_SELL,
};

UCLASS()
class MYRPG_API UShopWidget : public UPopupWidget
{
	GENERATED_BODY()
public:
	void Init(TWeakObjectPtr<class ANpc> NewNpc, TObjectPtr<class UMyGameInstance> Instance);


	UFUNCTION()
	void SetBuyMode();

	UFUNCTION()
	void SetSellMode();

	UFUNCTION()
	void CloseThisUI();

	UFUNCTION()
	void SellItems();

	virtual void Refresh() override;
public:
	void SetItemToBeSold(TObjectPtr<class UItem> Item);
	void RemoveItemToBeSold(TObjectPtr<class UItem> Item);

	ShopMode GetShopMode() { return Mode; }
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BuySlotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SellSlotClass;
private:
	/* 공용 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_BuyTap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_SellTap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	/* 구입창 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> BuyTap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> Scroll_BuyScroll;

	/* 판매창 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> SellTap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> Scroll_SellScroll;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Sell;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Total;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> InvenWidget;

private:
	// 판매 or 구매모드 추가
	ShopMode Mode;

	UPROPERTY()
	TMap<uint64, TObjectPtr<class UItem>> ItemsToBeSold;

	UPROPERTY()
	TMap<uint64, TObjectPtr<class USellSlotWidget>> SellSlots;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TWeakObjectPtr<class ANpc> OwnerNpc;

};
