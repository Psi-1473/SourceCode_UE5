// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataStruct.h"
#include "UI/DynamicInfo/DynamicInfoWidget.h"
#include "QuickQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuickQuestWidget : public UDynamicInfoWidget
{
	GENERATED_BODY()
	
public:
	void RegisterQuest(int32 QuestId, TObjectPtr<class UMyGameInstance> Instance, bool bSendPacket = true);

	void RemoveQuest(int32 QuestId);
	void RefreshElementUI(int32 QuestId);
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ElementClass;

	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UScrollBox> QuickQuestList;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TMap<int32, TObjectPtr<class UQuickQuestElementWidget>> QuickElementList;
};
