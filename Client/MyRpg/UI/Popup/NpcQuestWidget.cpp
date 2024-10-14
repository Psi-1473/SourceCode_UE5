// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/NpcQuestWidget.h"
#include "Npc.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "SubItem/QuestElementWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

void UNpcQuestWidget::Init(TWeakObjectPtr<class ANpc> Npc, TObjectPtr<UMyGameInstance> Instance)
{
	OwnerNpc = Npc;
	GInstance = Instance;
	Btn_Exit->OnClicked.AddDynamic(this, &UNpcQuestWidget::CloseThisUI);
	Refresh();
}

void UNpcQuestWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("NpcQuest"));
}

void UNpcQuestWidget::Refresh()
{
	TArray<int32> MainQuests = OwnerNpc->GetMainQuests();
	TArray<int32> NormalQuests = OwnerNpc->GetNormalQuests();
	TArray<int32> CompletableMainQuests = OwnerNpc->GetCompletableMainQuests();
	TArray<int32> CompletableNormalQuests = OwnerNpc->GetCompletableNormalQuests();
	
	CreateQuestElements(CompletableMainQuests, true);
	CreateQuestElements(CompletableNormalQuests, true);
	CreateQuestElements(MainQuests, false);
	CreateQuestElements(NormalQuests, false);
}

/*
 ========================================= private =========================================
*/
void UNpcQuestWidget::RemoveTargetsFromViewport()
{
	TArray<UWidget*> Children = ScrollBox_QuestList->GetAllChildren();
	for (UWidget* Child : Children)
	{
		if (Child)
		{
			Cast<UUserWidget>(Child)->RemoveFromViewport(); // 뷰포트에서 제거
		}
	}
	ScrollBox_QuestList->ClearChildren(); // 모든 자식 위젯 제거
}

void UNpcQuestWidget::CreateQuestElements(const TArray<int32>& Quests, bool isCompletable)
{
	for (int i = 0; i < Quests.Num(); i++)
	{
		TObjectPtr<UQuestElementWidget> Widget = Cast<UQuestElementWidget>(GInstance->GetUIMgr()->CreateSubItem(QuestSlotClass));
		ScrollBox_QuestList->AddChild(Widget);
		Widget->Init(OwnerNpc, GInstance, Quests[i], isCompletable);
	}
}
