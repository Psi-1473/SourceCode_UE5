// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(FString Name);
	void SetPercent(float Ratio);
	
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UProgressBar> Bar_Hp;

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	
};
