// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "CharacterSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetParentWidget(TWeakObjectPtr<class USelectCharacterWidget> Widget, TObjectPtr<class UMyGameInstance> Instance);
	void SetCharacter(const Protocol::BaseCharacterInfo& Info);
	void SetEmpty();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SelectThisSlot();

	void DeSelectThisSlot();

	void EnterGameWithThisSlot();

private:
	FString GetClassName(Protocol::ECharacterType Type);
private:
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCanvasPanel> Empty;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCanvasPanel> Character;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_NickName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Class;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Level;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Selected;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Character;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Class;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Select;

private:
	UPROPERTY()
	TWeakObjectPtr<class USelectCharacterWidget> Parent;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	uint64 CharacterId;

	
};
