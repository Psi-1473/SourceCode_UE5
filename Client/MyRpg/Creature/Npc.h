// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/DataStruct.h"
#include "Npc.generated.h"

UCLASS()
class MYRPG_API ANpc : public ACharacter
{
	GENERATED_BODY()

public:
	ANpc();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Init();
	void Interact();

	// 플레이어의 시작 가능한 퀘스트 추가
	void AddQuest(int32 QuestId);
	void AddCompletableQuest(int32 QuestId);
	void AddTargetQuest(int32 QuestId);

	// 플레이어의 시작 가능한 퀘스트 삭제
	void RemoveQuest(int32 QuestId);
	void RemoveCompletableQuest(int32 QuestId);
	void RemoveTargetQuest();

#pragma region OverlapFunctions
	UFUNCTION()
	void OnOInteractverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

public:
	int32 GetId() { return Id; }
	FNpcData* GetNpcData() { return MyData; }
	
	TObjectPtr<class UBoxComponent> GetInteractRange() { return InteractRange; }
	TArray<int32> GetCompletableMainQuests() { return CompletableMainQuests; }
	TArray<int32> GetCompletableNormalQuests() { return CompletableNormalQuests; }
	TArray<int32> GetMainQuests() { return MainQuest; }
	TArray<int32> GetNormalQuests() { return NormalQuest; }

private:
	void UpdateQuestMark();
	void SetInfoWidgetComponent();
	void SetRangeBox();
	void BindOverlapFunctions();
private:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> InteractRange;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> DetectionBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> NpcInfo;

	UPROPERTY()
	TArray<int32> CompletableMainQuests;

	UPROPERTY()
	TArray<int32> CompletableNormalQuests;


	UPROPERTY()
	TArray<int32> MainQuest;

	UPROPERTY()
	TArray<int32> NormalQuest;

	TQueue<int32> TargetQuest;

	int32 Id;
	FNpcData* MyData;
	
};
