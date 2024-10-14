// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "Protocol.pb.h"
#include "CreateCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UCreateCharacterWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void CheckDuplicate();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SelectWarrior();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SelectRogue();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SelectWizard();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void CreateCharacter();

	void SetDuplication(bool isDuplicated);

private:
	void SetVisibilityHidden();
	
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UEditableText> EText_Name;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Duplication;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Warrior;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Rogue;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Wizard;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Create;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Class;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Desc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_SelectWarrior;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_SelectRogue;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_SelectWizard;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
private:
	bool bDuplication = true;
	FString Name;
	Protocol::ECharacterType Type = Protocol::NONE;
	
	
};
