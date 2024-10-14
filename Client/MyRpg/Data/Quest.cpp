// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Quest.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "Data/DataStruct.h"

// temp
#include "Npc.h"
#include "UIManager.h"
#include "DynamicInfo/QuickQuestWidget.h"
UQuest::~UQuest()
{
	QuestTargets.Empty();
}

void UQuest::Init(int32 Id, TObjectPtr<UMyGameInstance> GInstance)
{
	// 1. 퀘스트 정보 세팅
	QuestId = Id;
	Info = GInstance->GetDataMgr()->GetQuestData(Id);

	// 2. 타겟 정보 세팅 
	InitTargetInfo(GInstance);
}

void UQuest::SetQuestTargetNumber(int32 TargetIndex, int32 TargetCount, TObjectPtr<UMyGameInstance> GInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("%d Quest's %d Target -> Now Target : %d"), QuestId, TargetIndex, TargetCount)

	QuestTargets[TargetIndex]->TargetNowNumber = TargetCount;
	
	// Temp ui 업데이트
	auto Widget = Cast<UQuickQuestWidget>(GInstance->GetUIMgr()->GetDynamicUI(TEXT("QuestQuick")));
	if (Widget != nullptr)
	{
		Widget->RefreshElementUI(QuestId);
	}

	if (QuestTargets[TargetIndex]->IsCompletable() == false && IsCompletable() == true)
	{
		// 완료 가능했다가 다시 불가능해짐
		GInstance->Npcs[GetData()->CompleteNpc]->RemoveCompletableQuest(QuestId);
		SetCompletable(false, GInstance);
	}
	//

	if (TargetCount == 0 && QuestTargets[TargetIndex]->TargetInfo->TargetType == Protocol::TARGET_NPC)
	{
		// 해당 엔피씨의 Target Quest에 추가
		GInstance->Npcs[QuestTargets[TargetIndex]->TargetInfo->TargetId]->AddTargetQuest(QuestId);
	}

}

TSharedPtr<FQuestTarget> UQuest::GetHuntTargetByTargetId(int32 TargetId)
{
	if(HuntTargets.Contains(TargetId) == false)
	{
		return nullptr;
	}

	return HuntTargets[TargetId];
}

TSharedPtr<FQuestTarget> UQuest::GetNpcTargetByTargetId(int32 TargetId)
{
	if (NpcTargets.Contains(TargetId) == false)
	{
		return nullptr;
	}

	return NpcTargets[TargetId];
}

void UQuest::SetCompletable(bool Value, TObjectPtr<UMyGameInstance> GInstance)
{
	bCompletable = Value;
	// Temp
	if (Value == true)
	{
		auto Widget = Cast<UQuickQuestWidget>(GInstance->GetUIMgr()->GetDynamicUI(TEXT("QuestQuick")));
		if (Widget != nullptr)
		{
			Widget->RefreshElementUI(QuestId);
		}
	}
}

void UQuest::InitTargetInfo(TObjectPtr<UMyGameInstance> GInstance)
{
	TSharedPtr<FQuestTargets> TargetInfo = GInstance->GetDataMgr()->GetQuestTargetData(QuestId);
	if (TargetInfo == nullptr)
	{
		bCompletable = true;
		return;
	}
	//if (TargetInfo->Targets.Num() == 0)
	//{
	//	bCompletable = true;
	//	return;
	//}
	for (auto Target : TargetInfo->Targets)
	{
		TSharedPtr<FQuestTarget> MyTarget = MakeShared<FQuestTarget>();
		MyTarget->TargetInfo = Target;
		MyTarget->TargetNowNumber = 0;
		QuestTargets.Add(MyTarget);

		if (Target->TargetType == Protocol::TARGET_MONSTER)
		{
			HuntTargets.Add(Target->TargetId, MyTarget);
		}
		else if (Target->TargetType == Protocol::TARGET_NPC)
		{
			NpcTargets.Add(Target->TargetId, MyTarget);
		}
		else if (Target->TargetType == Protocol::TARGET_CONSUMABLE)
		{
			ConsumableTargets.Add(Target->TargetId, MyTarget);
		}
	}
}

