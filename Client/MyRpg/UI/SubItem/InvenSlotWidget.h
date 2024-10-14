// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "InvenSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UInvenSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInvenSlotWidget(const FObjectInitializer& ObjectInitializer);
	void Init(TObjectPtr<class UMyGameInstance> Instance, TWeakObjectPtr<class UInventoryWidget> Widget, int32 Index);


	virtual void NativeConstruct();
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

public:
	TWeakObjectPtr<class UInventoryWidget> GetBaseInventory() { return InventoryWidget; }
	int32 GetSlotIndex() { return SlotIndex; }
	TObjectPtr<class UItem> GetItem() { return Item; }

	void SetItem(TObjectPtr<class UItem> NewItem);
	void SetInfo();
	void SetNull();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DragVisualClass;

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_Number;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Item;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_ToBeSold;

private:
	UPROPERTY()
	TObjectPtr<class UItem> Item;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TWeakObjectPtr<class UInventoryWidget> InventoryWidget;
	
	UPROPERTY()
	int32 SlotIndex;
};
