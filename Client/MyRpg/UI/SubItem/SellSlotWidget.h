// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Blueprint/UserWidget.h"
#include "SellSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USellSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UItem> NewItem, TWeakObjectPtr<class UShopWidget> NewShopSlot);
	void SetInfo();

	UFUNCTION()
	void Cancle();
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UImage> Img_Item;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_ItemName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Gold;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Cancle;

private:

	UPROPERTY()
	TObjectPtr<class UItem> Item;

	UPROPERTY()
	TWeakObjectPtr<class UShopWidget> ShopWidget;
};
