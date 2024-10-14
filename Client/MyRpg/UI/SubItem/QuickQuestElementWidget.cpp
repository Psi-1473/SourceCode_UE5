// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuickQuestElementWidget.h"
#include "UI/SubItem/QuickQuestTargetWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "MyPlayer.h"
#include "ActorComponents/QuestComponent.h"
#include "Data/Quest.h"
#include "SubItem/QuestTargetWidget.h"

#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/StackBox.h"

void UQuickQuestElementWidget::SetQuestInfo(int32 QuestId, TObjectPtr<UMyGameInstance> Instance)
{
	// 1. 퀘스트 타겟 정보 가져오기
	GInstance = Instance;
	QuestInfo = GInstance->GetPlayer()->GetQuestComponent()->GetInPrgressQuests()[QuestId];

	FString RichText = TEXT("");
	if (QuestInfo->GetData()->QuestGrade == Protocol::QUEST_MAIN)
	{
		RichText = TEXT("<MainQuest>[메인]</> ");
	}
	else if(QuestInfo->GetData()->QuestGrade == Protocol::QUEST_NORMAL)
	{
		RichText = TEXT("<NormalQuest>[일반]</> ");
	}
	RichText += QuestInfo->GetData()->QuestName;
	
	Text_QuestName->SetText(FText::FromString(RichText));

	if (QuestInfo->IsCompletable())
	{
		SetQuestInfoCompleted();
		return;
	}

	CreateTargetInfo();
}

void UQuickQuestElementWidget::Refresh()
{
	if (QuestInfo->IsItemQuest() == false)
	{
		if (QuestInfo->IsCompletable())
		{
			SetQuestInfoCompleted();
			bCleared = true;
			return;
		}
	}
	

	for (auto& TargetWidget : TargetWidgetList)
	{
		TargetWidget->Refresh();
	}

}

void UQuickQuestElementWidget::CreateTargetInfo()
{
	auto Targets = QuestInfo->GetTargets();
	for (int i = 0; i < Targets.Num(); i++)
	{
		auto Widget = Cast<UQuickQuestTargetWidget>(GInstance->GetUIMgr()->CreateSubItem(TargetClass));
		Widget->SetQuestInfo(Targets[i], GInstance);
		Stack_Target->AddChild(Widget);
		TargetWidgetList.Add(Widget);
	}
}
void UQuickQuestElementWidget::SetQuestInfoCompleted()
{
	

	for (auto& TargetWidget : TargetWidgetList)
	{
		TargetWidget->RemoveFromParent();
	}
	auto Widget = Cast<UQuickQuestTargetWidget>(GInstance->GetUIMgr()->CreateSubItem(TargetClass));
	Widget->SetCompletableNpcInfo(QuestInfo->GetData()->CompleteNpc, GInstance);
	Stack_Target->AddChild(Widget);
	TargetWidgetList.Add(Widget);
}
