// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuickQuestTargetWidget.h"
#include "DataManager.h"
#include "MyGameInstance.h"
#include "Data/Quest.h"
#include "Protocol.pb.h"

#include "Components/TextBlock.h"

void UQuickQuestTargetWidget::SetQuestInfo(TSharedPtr<FQuestTarget> Info, TObjectPtr<UMyGameInstance> Instance)
{
	TargetInfo = Info;
	GInstance = Instance;
	Refresh();
}

void UQuickQuestTargetWidget::SetCompletableNpcInfo(int32 NpcId, TObjectPtr<UMyGameInstance> Instance)
{
	FString Text = Instance->GetDataMgr()->GetNpcData(NpcId)->NpcName;
	Text += TEXT("에게 찾아가기");
	Text_TargetInfo->SetText(FText::FromString(Text));
}

void UQuickQuestTargetWidget::Refresh()
{
	FString TargetText = TEXT("");
	if (TargetInfo->TargetInfo->TargetType == Protocol::TARGET_MONSTER)
	{
		TargetText = GInstance->GetDataMgr()->GetMonsterStatData(TargetInfo->TargetInfo->TargetId)->mobName;
		TargetText += TEXT(" 처치  ");

		TargetText += FString::FromInt(TargetInfo->TargetNowNumber) + TEXT(" / ") + FString::FromInt(TargetInfo->TargetInfo->TargetNumber);
	}
	else if(TargetInfo->TargetInfo->TargetType == Protocol::TARGET_NPC)
	{
		TargetText = GInstance->GetDataMgr()->GetNpcData(TargetInfo->TargetInfo->TargetId)->NpcName;
		TargetText += TEXT("와 대화하기");
	}
	else if (TargetInfo->TargetInfo->TargetType == Protocol::TARGET_CONSUMABLE)
	{
		TargetText = GInstance->GetDataMgr()->GetConsumableData(TargetInfo->TargetInfo->TargetId)->ItemName;
		TargetText += TEXT(" 얻기  ");
		TargetText += FString::FromInt(TargetInfo->TargetNowNumber) + TEXT(" / ") + FString::FromInt(TargetInfo->TargetInfo->TargetNumber);

	}

	Text_TargetInfo->SetText(FText::FromString(TargetText));

	if (IsCleared())
	{
		Text_TargetInfo->SetColorAndOpacity(FColor(2.0f, 2.0f, 2.0f, 255.0f));
	}
	else
	{
		Text_TargetInfo->SetColorAndOpacity(FColor(255.0f, 255.0f, 255.0f, 255.0f));
	}
}

bool UQuickQuestTargetWidget::IsCleared()
{
	return TargetInfo->TargetNowNumber >= TargetInfo->TargetInfo->TargetNumber;
}
