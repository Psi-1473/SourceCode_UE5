// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuestElementWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(TWeakObjectPtr<class ANpc> Npc, TObjectPtr<class UMyGameInstance> Instance, int32 NewQuestId, bool IsCompletable);

	UFUNCTION()
	void PopupQuestInfoWidget();

private:
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_QuestName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Completable;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Quest;

	UPROPERTY()
	TWeakObjectPtr<class ANpc> OwnerNpc;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	
	int32 QuestId;
	bool bCompletable;
};
