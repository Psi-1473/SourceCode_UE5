// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Protocol.pb.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPG_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestComponent();
	void Init(TObjectPtr<class AMyPlayer> Player);

protected:
	virtual void BeginPlay() override;

public:	
	TMap<int32, TObjectPtr<class UQuest>> GetInPrgressQuests() { return InProgressQuests; }
	void AcceptNewQuest(int32 QuestId, int32 TargetCount = 0);
	void AbandonQuest(int32 QuestId);
	void CompleteQuest(int32 QuestId);
	void SetQuestCompletable(int32 QuestId, bool Value);

	//bool HasTargetQuest(Protocol::ETargetType Type, int32 TargetId);
private:
	TObjectPtr<class UQuest> CreateQuest(int32 QuestId);

private:
	// 진행 중인 목록만 들고있기
	UPROPERTY()
	TMap<int32, TObjectPtr<class UQuest>> InProgressQuests;
	
	UPROPERTY()
	TWeakObjectPtr<class AMyPlayer> OwnerPlayer;
		
};
