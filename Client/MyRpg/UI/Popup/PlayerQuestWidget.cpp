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
	// 1. ����Ʈ ���� UI�� ���� �ش� UI�� �ؽ�Ʈ�� �������� �ش� ����Ʈ ������ ����
	OpenAndSetQuestDescWidget(QuestId);

	// 2. ����Ʈ Ÿ�� ���� �����ϱ�
	// 2-1) ������ ���õ� ����Ʈ Ÿ���� ������ �켱 ��� ����
	RemoveTargetsFromViewport();
	// 2-2) QuestId�� ���� ǥ���Ϸ��� ����Ʈ�� Ÿ�� ������ Widget�� ǥ��.
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
			Cast<UUserWidget>(Child)->RemoveFromViewport(); // ����Ʈ���� ����
		}
	}
	TargetBox->ClearChildren(); // ��� �ڽ� ���� ����
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
