// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UQuestManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);

	void AddAvailableQuest(struct FQuestData* QuestData);
	void AcceptQuest(FQuestData* QuestData);
	void SetQuestCompletable(FQuestData* QuestData);
	void CompleteQuest(FQuestData* QuestData);

private:
	// "Npc와 대화하기" 같이 특정 Npc가 타겟이 되는 퀘스트가 있다면 해당 Npc 정보 업데이트
	void TargetNpcQuest(int32 QuestId);

	/* 퀘스트 관련 UI 갱신하는 함수 */
	void RemoveQuestFromQuickWidget(int32 QuestId);
private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
