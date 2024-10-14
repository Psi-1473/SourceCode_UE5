// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickQuestTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuickQuestTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetQuestInfo(TSharedPtr<struct FQuestTarget> Info, TObjectPtr<class UMyGameInstance> Instance);
	void SetCompletableNpcInfo(int32 NpcId, TObjectPtr<class UMyGameInstance> Instance);

	void Refresh();
	bool IsCleared();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_TargetInfo;

private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	TSharedPtr<FQuestTarget> TargetInfo;
	
	
};
