// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "NpcQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UNpcQuestWidget : public UPopupWidget
{
	GENERATED_BODY()
public:
	void Init(TWeakObjectPtr<class ANpc> Npc, TObjectPtr<class UMyGameInstance> Instance);

	UFUNCTION()
	void CloseThisUI();

	virtual void Refresh() override;

private:
	void RemoveTargetsFromViewport();
	void CreateQuestElements(const TArray<int32>& Quests, bool isCompletable);
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> QuestSlotClass;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_QuestList;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY()
	TWeakObjectPtr<class ANpc> OwnerNpc;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

};
