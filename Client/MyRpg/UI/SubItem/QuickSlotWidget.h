// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetKey();
	
	void Use(TObjectPtr<class AMyPlayer> Player);

	void SetItem(TObjectPtr<class UItem> NewItem);
	void SetSkill(TObjectPtr<class USkill> NewSkill);
	void Empty();
	void Refresh();

	void SetCoolDown(float Value) { CoolDown = Value; MaxCoolDown = Value;}
private:
	void RemovePrevElement();
	void SetElement(TObjectPtr<class USlotDrag> Drag);
	void UseItem();
	void UseSkill(TObjectPtr<class AMyPlayer> Player);
	void SendRegisterPakcet(Protocol::EQuickElementType ElementType);

	Protocol::EQuickSlot GetQuickType();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_Slot;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Count;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Item;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Cooldown;


private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TObjectPtr<class UItem> Item;

	UPROPERTY()
	TObjectPtr<class USkill> Skill;
	
	float CoolDown = 0.0f;
	float MaxCoolDown = 0.0f;
};
