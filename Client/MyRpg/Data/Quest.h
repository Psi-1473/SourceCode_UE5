// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataStruct.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuestTarget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 TargetNowNumber;

	TSharedPtr<FQuestTargetData> TargetInfo;

	bool IsCompletable() { return TargetNowNumber >= TargetInfo->TargetNumber; }
};

UCLASS()
class MYRPG_API UQuest : public UObject
{
	GENERATED_BODY()
public:
	~UQuest();
	// 퀘스트 클래스를 메모리에 할당 후 실행할 초기화 함수
	void Init(int32 Id, TObjectPtr<class UMyGameInstance> GInstance);
	void SetQuestTargetNumber(int32 TargetIndex, int32 TargetCount, TObjectPtr<class UMyGameInstance> GInstance);

	void SetCompletable(bool Value, TObjectPtr<class UMyGameInstance> GInstance);
public:
	struct FQuestData* GetData() { return Info; }
	bool IsCompletable() { return bCompletable; }
	bool IsItemQuest() { return ConsumableTargets.Num() > 0; }
	const TArray<TSharedPtr<FQuestTarget>>& GetTargets() { return QuestTargets; }
	TSharedPtr<FQuestTarget> GetTargetByIndex(int32 Index) { return QuestTargets[Index]; }
	TSharedPtr<FQuestTarget> GetHuntTargetByTargetId(int32 TargetId);
	TSharedPtr<FQuestTarget> GetNpcTargetByTargetId(int32 TargetId);

private:
	void InitTargetInfo(TObjectPtr<class UMyGameInstance> GInstance);
private:
	int32 QuestId;
	struct FQuestData* Info;
	
	TArray<TSharedPtr<FQuestTarget>> QuestTargets;
	TMap<int32, TSharedPtr<FQuestTarget>> HuntTargets;
	TMap<int32, TSharedPtr<FQuestTarget>> NpcTargets;
	TMap<int32, TSharedPtr<FQuestTarget>> ConsumableTargets;
	bool bCompletable;
};
