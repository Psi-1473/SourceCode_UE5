// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "AlarmWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UAlarmWidget : public UPopupWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetText(FString Text);

	UFUNCTION()
	void CloseThisUI();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_Alarm;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Confirm;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
