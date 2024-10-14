// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "Data/Quest.h"

// Temp
#include "UIManager.h"
#include "DynamicInfo/QuickQuestWidget.h"

UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestComponent::Init(TObjectPtr<class AMyPlayer> Player)
{
	OwnerPlayer = Player;
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UQuestComponent::AcceptNewQuest(int32 QuestId, int32 TargetCount)
{
	InProgressQuests.Add(QuestId, CreateQuest(QuestId));
}

void UQuestComponent::AbandonQuest(int32 QuestId)
{
}

void UQuestComponent::CompleteQuest(int32 QuestId)
{
	InProgressQuests.Remove(QuestId);
	UE_LOG(LogTemp, Warning, TEXT("Quest Clear!"));


}

void UQuestComponent::SetQuestCompletable(int32 QuestId, bool Value)
{
	if (InProgressQuests.Contains(QuestId) == false)
	{
		return;
	}
	InProgressQuests[QuestId]->SetCompletable(Value, Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance()));
}

TObjectPtr<class UQuest> UQuestComponent::CreateQuest(int32 QuestId)
{
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(OwnerPlayer->GetGameInstance());
	TObjectPtr<UQuest> NewQuest = NewObject<UQuest>();
	NewQuest->Init(QuestId, GInstance);
	
	return NewQuest;
}



