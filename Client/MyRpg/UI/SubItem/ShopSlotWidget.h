// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "ShopSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UShopSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(Protocol::ENpcType NpcType, int ItemId, TObjectPtr<class UMyGameInstance> Instance);

	UFUNCTION()
	void Click_Buy();

private:
	void SetWeapon(int ItemId);
	void SetArmor(int ItemId);
	void SetConsumable(int ItemId);



private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Price;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Buy;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Img_Item;
private:
	// 아이템 클래스 만들면 해당 아이템 정보 변수로 들고있다가 구매하면 넘기기

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	Protocol::EItemType ItemType;
	uint32 TemplateId;
	int32 BuyPrice;
};
