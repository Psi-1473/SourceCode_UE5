// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UItemInfoWidget : public UPopupWidget
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr<class UItem> BaseItem, TObjectPtr<UUserWidget> BaseWidget, FString WidgetType, TObjectPtr<class UMyGameInstance> Instance);
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class URichTextBlock> RText_ItemName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> RText_ItemGrade;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Item;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_PowerType;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_ClassType;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Power;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> RText_ItemDesc;
private:
	UPROPERTY()
	TObjectPtr<class UItem> Item;
};
