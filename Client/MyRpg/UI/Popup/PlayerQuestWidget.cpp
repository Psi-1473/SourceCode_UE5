// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/PlayerQuestWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "DataManager.h"
#include "MyPlayer.h"
#include "Data/DataStruct.h"
#include "Data/Quest.h"
#include "ActorComponents/QuestComponent.h"
#include "SubItem/QuestElementWidget.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "SubItem/QuestTargetWidget.h"
#include "DynamicInfo/QuickQuestWidget.h"

void UPlayerQuestWidget::Init(TObjectPtr<UMyGameInstance> Instance)
{
	GInstance = Instance;
	Btn_Exit->OnClicked.AddDynamic(this, &UPlayerQuestWidget::CloseThisUI);
	Btn_RegisterQuick->OnClicked.AddDynamic(this, &UPlayerQuestWidget::RegisterQuickQuest);
	Btn_HideDesc->OnClicked.AddDynamic(this, &UPlayerQuestWidget::HideDescUI);
	Refresh();
}

void UPlayerQuestWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("PlayerQuest"));
}

void UPlayerQuestWidget::HideDescUI()
{
	Desc->SetVisibility(ESlateVisibility::Hidden);
	SelectedQuest = nullptr;
}

void UPlayerQuestWidget::RegisterQuickQuest()
{
	if (SelectedQuest == nullptr)
	{
		return;
	}
	TObjectPtr<UQuickQuestWidget> Widget = Cast<UQuickQuestWidget>(GInstance->GetUIMgr()->GetDynamicUI(TEXT("QuestQuick")));
	if(Widget == nullptr)
	{
		Widget = Cast<UQuickQuestWidget>(GInstance->GetUIMgr()->PopupDynamicInfoUI(TEXT("QuestQuick")));
	}
	Widget->RegisterQuest(SelectedQuest->QuestId, GInstance);

}

void UPlayerQuestWidget::SetDescWidget(int32 QuestId)
{
	// 1. 퀘스트 설명 UI를 띄우고 해당 UI의 텍스트나 정보들을 해당 퀘스트 정보로 세팅
	OpenAndSetQuestDescWidget(QuestId);

	// 2. 퀘스트 타겟 정보 세팅하기
	// 2-1) 기존에 세팅된 퀘스트 타겟이 있으면 우선 모두 제거
	RemoveTargetsFromViewport();
	// 2-2) QuestId를 통해 표시하려는 퀘스트의 타겟 정보를 Widget에 표시.
	SetTargetInfoWithDescWidget(QuestId);
}

void UPlayerQuestWidget::Refresh()
{
	HideDescUI();
	SetInProgressQuests();
}

void UPlayerQuestWidget::OpenAndSetQuestDescWidget(int32 QuestId)
{
	FQuestData* Quest = GInstance->GetDataMgr()->GetQuestData(QuestId);
	SelectedQuest = Quest;
	Desc->SetVisibility(ESlateVisibility::Visible);
	Text_QuestName->SetText(FText::FromString(Quest->QuestName));
	Text_QuestDesc->SetText(FText::FromString(Quest->QuestDesc));
}
void UPlayerQuestWidget::RemoveTargetsFromViewport()
{
	TArray<UWidget*> Children = TargetBox->GetAllChildren();
	for (UWidget* Child : Children)
	{
		if (Child)
		{
			Cast<UUserWidget>(Child)->RemoveFromViewport(); // 뷰포트에서 제거
		}
	}
	TargetBox->ClearChildren(); // 모든 자식 위젯 제거
}
void UPlayerQuestWidget::SetTargetInfoWithDescWidget(int32 QuestId)
{
	TSharedPtr<FQuestTargets> TargetInfo = GInstance->GetDataMgr()->GetQuestTargetData(QuestId);
	if (TargetInfo == nullptr)
	{
		return;
	}
	for (int i = 0; i < TargetInfo->Targets.Num(); i++)
	{
		TObjectPtr<UQuestTargetWidget> Widget = Cast<UQuestTargetWidget>(GInstance->GetUIMgr()->CreateSubItem(TargetSlotClass));
		TargetBox->AddChild(Widget);
		Widget->Init(GInstance->MyPlayer->GetQuestComponent()->GetInPrgressQuests()[QuestId], i);
	}
}
void UPlayerQuestWidget::SetInProgressQuests()
{
	for (auto& Quest : GInstance->MyPlayer->GetQuestComponent()->GetInPrgressQuests())
	{
		TObjectPtr<UQuestElementWidget> Widget = Cast<UQuestElementWidget>(GInstance->GetUIMgr()->CreateSubItem(QuestSlotClass));
		ScrollBox_QuestList->AddChild(Widget);
		Widget->Init(nullptr, GInstance, Quest.Key, Quest.Value->IsCompletable());
	}
}
