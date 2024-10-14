// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "NpcQuestInfo.generated.h"

/**
 * 
 */
enum class RewardType
{
	EXP,
	GOLD,
	ITEM
};
UCLASS()
class MYRPG_API UNpcQuestInfo : public UPopupWidget
{
	GENERATED_BODY()

public:
	void Init(TWeakObjectPtr<class ANpc> Npc, TObjectPtr<class UMyGameInstance> Instance, int32 NewQuestId, bool IsCompletable);

	UFUNCTION()
	void CloseThisUI();

	UFUNCTION()
	void AcceptQuest();

private:
	void SetBaseInfo();
	void SetRewardInfo();

	TObjectPtr<class UQuestRewardWidget> CreateRewardWidget();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> RewardClass;

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_QuestName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_QuestDesc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Accept;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Accept;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> HorizontalBox_Rewards;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
	int32 QuestId;
	bool bCompletable;
	
};
