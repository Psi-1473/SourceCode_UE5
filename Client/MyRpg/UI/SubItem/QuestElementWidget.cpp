// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuestElementWidget.h"
#include "Npc.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "UIManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Popup/NpcQuestInfo.h"
#include "Popup/PlayerQuestWidget.h"
#include "Popup/NpcWidget.h"

void UQuestElementWidget::Init(TWeakObjectPtr<ANpc> Npc, TObjectPtr<UMyGameInstance> Instance, int32 NewQuestId, bool IsCompletable)
{
	OwnerNpc = Npc;
	GInstance = Instance;
	QuestId = NewQuestId;
	bCompletable = IsCompletable;
	FQuestData* Data = GInstance->GetDataMgr()->GetQuestData(QuestId);
	Text_QuestName->SetText(FText::FromString(Data->QuestName));
	Btn_Quest->OnClicked.AddDynamic(this, &UQuestElementWidget::PopupQuestInfoWidget);

	if (bCompletable)
	{
		Text_Completable->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_Completable->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuestElementWidget::PopupQuestInfoWidget()
{
	if (OwnerNpc == nullptr)
	{
		auto Widget = Cast<UPlayerQuestWidget>(GInstance->GetUIMgr()->GetPopupUI(TEXT("PlayerQuest")));
		Widget->SetDescWidget(QuestId);
	}
	else
	{
		// 1. Npc Quest À§Á¬ ´Ý±â
		GInstance->GetUIMgr()->CloseUI(TEXT("NpcQuest"));
		auto Widget = Cast<UNpcWidget>(GInstance->GetUIMgr()->GetPopupUI(TEXT("Npc")));

		if(bCompletable)
		{
			Widget->SetDialogueMode(QuestId, 1);
		}
		else
		{
			Widget->SetDialogueMode(QuestId, 0);
		}

		// 2. ½ºÅ©¸³Æ® ½ÇÇà
		//auto Widget = Cast<UNpcQuestInfo>(GInstance->GetUIMgr()->PopupUI(TEXT("NpcQuestInfo")));
		//Widget->Init(OwnerNpc, GInstance, QuestId, bCompletable);
	}
}
