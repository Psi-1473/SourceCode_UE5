// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuestTargetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr<class UQuest> NewQuest, int32 TargetIndex);

	void Refresh();
private:
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_TargetDesc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_NowTarget;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_MaxTarget;

	UPROPERTY()
	TObjectPtr<class UQuest> Quest;

	int32 Index;
};
