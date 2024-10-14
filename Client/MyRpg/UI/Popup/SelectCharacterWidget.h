// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "Protocol.pb.h"

#include "SelectCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USelectCharacterWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	
	USelectCharacterWidget(const FObjectInitializer& ObjectInitializer);
	void Init(TObjectPtr<class UMyGameInstance> Instance);
	void SetSlot(const Protocol::S_LOGIN& Pkt);

	void SetSlotsDeselected();
	void SetSelectedSlot(TObjectPtr<class UCharacterSlotWidget> NewSlot) { SelectedSlot = NewSlot; }

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void EnterGame();
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void PopupCreateWidget();
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UButton> Btn_Create;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Start;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCharacterSlotWidget> CharacterSlot1;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCharacterSlotWidget> CharacterSlot2;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCharacterSlotWidget> CharacterSlot3;

	UPROPERTY()
	TArray<TObjectPtr<class UCharacterSlotWidget>> Slots;

	UPROPERTY()
	TObjectPtr<class UCharacterSlotWidget> SelectedSlot;
private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
