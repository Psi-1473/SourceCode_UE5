// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UEquipSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr<class UMyGameInstance> Instance, TObjectPtr<class UItem> NewItem);

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
private:
	void SetInfo();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UImage> Img_Item;
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> BaseImage;

	UPROPERTY()
	TObjectPtr<class UItem> Item;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
	
};
