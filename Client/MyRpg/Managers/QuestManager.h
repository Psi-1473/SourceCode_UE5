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
	// "Npc�� ��ȭ�ϱ�" ���� Ư�� Npc�� Ÿ���� �Ǵ� ����Ʈ�� �ִٸ� �ش� Npc ���� ������Ʈ
	void TargetNpcQuest(int32 QuestId);

	/* ����Ʈ ���� UI �����ϴ� �Լ� */
	void RemoveQuestFromQuickWidget(int32 QuestId);
private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
