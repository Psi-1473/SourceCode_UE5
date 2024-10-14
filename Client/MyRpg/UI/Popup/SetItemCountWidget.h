// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "SetItemCountWidget.generated.h"

/**
 * 
 */

UCLASS()
class MYRPG_API USetItemCountWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UMyGameInstance> Instance, TObjectPtr<class UItem> NewItem, TWeakObjectPtr<class UShopWidget> NewShopWidget);

	UFUNCTION()
	void CloseThisUI();

	UFUNCTION()
	void Confirm();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Confirm;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEditableText> EText_Input;

private:
	UPROPERTY()
	TObjectPtr<class UItem> Item;

	UPROPERTY()
	TWeakObjectPtr<class UShopWidget> ShopWidget;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
