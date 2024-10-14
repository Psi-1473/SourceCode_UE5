// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USkillWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);
	virtual void NativeDestruct() override;

	UFUNCTION()
	void CloseThisUI();

	UFUNCTION()
	void Refresh();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Point;
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	FDelegateHandle Handle;
};
