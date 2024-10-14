// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Blueprint/UserWidget.h"
#include "QuestRewardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuestRewardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetExp(int32 Exp, TObjectPtr<class UMyGameInstance> Instance);
	void SetGold(int32 Gold, TObjectPtr<class UMyGameInstance> Instance);
	void SetItem(Protocol::EItemType ItemType, int32 ItemId, int32 Count, TObjectPtr<class UMyGameInstance> Instance);
private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UImage> Img_Reward;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Value;
};
