// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/QuestManager.h"
#include "Data/DataStruct.h"
#include "DataManager.h"
#include "UIManager.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "Npc.h"
#include "ActorComponents/QuestComponent.h"

// Äù½ºÆ® °ü·Ã À§Á¬
#include "DynamicInfo/QuickQuestWidget.h"

void UQuestManager::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
}

void UQuestManager::AddAvailableQuest(FQuestData* QuestData)
{
	int32 StartNpcId = QuestData->StartNpc;
	int32 QuestId = QuestData->QuestId;
	GInstance->GetNpc(QuestData->StartNpc)->AddQuest(QuestData->QuestId);
}

void UQuestManager::AcceptQuest(FQuestData* QuestData)
{
	int32 StartNpcId = QuestData->StartNpc;
	int32 QuestId = QuestData->QuestId;

	GInstance->GetNpc(StartNpcId)->RemoveQuest(QuestId);
	GInstance->GetPlayer()->GetQuestComponent()->AcceptNewQuest(QuestId);

	TargetNpcQuest(QuestId);
}

void UQuestManager::SetQuestCompletable(FQuestData* QuestData)
{
	int32 CompleteNpcId = QuestData->CompleteNpc;
	int32 QuestId = QuestData->QuestId;

	GInstance->GetPlayer()->GetQuestComponent()->SetQuestCompletable(QuestId, true);
	GInstance->GetNpc(CompleteNpcId)->AddCompletableQuest(QuestId);
}

void UQuestManager::CompleteQuest(FQuestData* QuestData)
{
	int32 CompleteNpcId = QuestData->CompleteNpc;
	int32 QuestId = QuestData->QuestId;

	GInstance->GetNpc(CompleteNpcId)->RemoveCompletableQuest(QuestId);
	GInstance->GetPlayer()->GetQuestComponent()->CompleteQuest(QuestId);

	RemoveQuestFromQuickWidget(QuestId);
}

void UQuestManager::TargetNpcQuest(int32 QuestId)
{
	TSharedPtr<FQuestTargets> Targets = GInstance->GetDataMgr()->GetQuestTargetData(QuestId);

	if (Targets == nullptr)
	{
		return;
	}

	for (int j = 0; j < Targets->Targets.Num(); j++)
	{
		if (Targets->Targets[j]->TargetType == Protocol::TARGET_NPC)
		{
			int32 TargetNpcId = Targets->Targets[j]->TargetId;
			GInstance->GetNpc(TargetNpcId)->AddTargetQuest(QuestId);
		}
	}
}
void UQuestManager::RemoveQuestFromQuickWidget(int32 QuestId)
{

	auto Widget = Cast<UQuickQuestWidget>(GInstance->GetUIMgr()->GetDynamicUI(TEXT("QuestQuick")));
	if (Widget != nullptr)
	{
		Widget->RemoveQuest(QuestId);
	}
}
