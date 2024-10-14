// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickQuestElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuickQuestElementWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuestInfo(int32 QuestId, TObjectPtr<class UMyGameInstance> Instance);
	void Refresh();

private:
	void CreateTargetInfo();
	void SetQuestInfoCompleted();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TargetClass;

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class URichTextBlock> Text_QuestName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UStackBox> Stack_Target;

private:
	UPROPERTY()
	TArray<TObjectPtr<class UQuickQuestTargetWidget>> TargetWidgetList;

	UPROPERTY()
	TObjectPtr<class UQuest> QuestInfo;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	bool bCleared = false;
};
