// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuestTargetWidget.h"
#include "Data/Quest.h"
#include "Components/TextBlock.h"

void UQuestTargetWidget::Init(TObjectPtr<class UQuest> NewQuest, int32 TargetIndex)
{
	Quest = NewQuest;
	Index = TargetIndex;
	Refresh();
}

void UQuestTargetWidget::Refresh()
{
	TSharedPtr<FQuestTarget> TargetInfo = Quest->GetTargetByIndex(Index);

	Text_TargetDesc->SetText(FText::FromString(TargetInfo->TargetInfo->TargetDesc));
	Text_NowTarget->SetText(FText::FromString(FString::FromInt(TargetInfo->TargetNowNumber)));
	Text_MaxTarget->SetText(FText::FromString(FString::FromInt(TargetInfo->TargetInfo->TargetNumber)));
}
