// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuestRewardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "MyGameInstance.h"
#include "DataManager.h"

void UQuestRewardWidget::SetExp(int32 Exp, TObjectPtr<UMyGameInstance> Instance)
{
	Text_Value->SetText(FText::FromString(FString::FromInt(Exp)));
	Img_Reward->SetBrush(Instance->GetDataMgr()->GetGeneralImage(TEXT("Exp"))->Brush);
}

void UQuestRewardWidget::SetGold(int32 Gold, TObjectPtr<UMyGameInstance> Instance)
{
	Text_Value->SetText(FText::FromString(FString::FromInt(Gold)));
	Img_Reward->SetBrush(Instance->GetDataMgr()->GetGeneralImage(TEXT("Gold"))->Brush);
}

void UQuestRewardWidget::SetItem(Protocol::EItemType ItemType, int32 ItemId, int32 Count, TObjectPtr<UMyGameInstance> Instance)
{
	if (ItemType == Protocol::ITEM_ARMOR)
	{
		Img_Reward->SetBrush(Instance->GetDataMgr()->GetArmorImage(ItemId)->Brush);
	}
	else if (ItemType == Protocol::ITEM_WEAPON)
	{
		Img_Reward->SetBrush(Instance->GetDataMgr()->GetWeaponImage(ItemId)->Brush);
	}
	else if (ItemType == Protocol::ITEM_CONSUMABLE)
	{
		Img_Reward->SetBrush(Instance->GetDataMgr()->GetConsumableImage(ItemId)->Brush);
	}

	
	if (Count == 1)
	{
		Text_Value->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		Text_Value->SetText(FText::FromString(FString::FromInt(Count)));
	}
	
}
