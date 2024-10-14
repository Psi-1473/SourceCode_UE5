// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/NpcInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/DataStruct.h"

void UNpcInfoWidget::Init(FNpcData* NpcData)
{
	Text_Title->SetText(FText::FromString(NpcData->Title));
	Text_Name->SetText(FText::FromString(NpcData->NpcName));
	RemoveQuestMark();
	RemoveCompletableMark();
	RemoveTargetMark();
}

void UNpcInfoWidget::DrawQuestMark(Protocol::EQuestGrade Grade)
{
	RemoveCompletableMark();
	RemoveTargetMark();
	Img_QuestMark->SetVisibility(ESlateVisibility::Visible);

	if (Grade == Protocol::QUEST_MAIN)
	{
		//Img_QuestMark->SetBrushTintColor(FSlateColor(FColor(1, )))
		Img_QuestMark->ColorAndOpacity.R = 1.0f;
		Img_QuestMark->ColorAndOpacity.G = 0.7f;
		Img_QuestMark->ColorAndOpacity.B = 0.f;
		Img_QuestMark->ColorAndOpacity.A = 1.0f;
	}
	else if (Grade == Protocol::QUEST_NORMAL)
	{
		Img_QuestMark->ColorAndOpacity.R = 0.f;
		Img_QuestMark->ColorAndOpacity.G = 1.0f;
		Img_QuestMark->ColorAndOpacity.B = 0.f;
		Img_QuestMark->ColorAndOpacity.A = 1.0f;
	}
}

void UNpcInfoWidget::DrawCompletableMark(Protocol::EQuestGrade Grade)
{
	RemoveQuestMark();
	RemoveTargetMark();
	Img_CompletableMark->SetVisibility(ESlateVisibility::Visible);

	if (Grade == Protocol::QUEST_MAIN)
	{
		//Img_QuestMark->SetBrushTintColor(FSlateColor(FColor(1, )))
		Img_QuestMark->ColorAndOpacity.R = 1.0f;
		Img_QuestMark->ColorAndOpacity.G = 0.7f;
		Img_QuestMark->ColorAndOpacity.B = 0.f;
		Img_QuestMark->ColorAndOpacity.A = 1.0f;
	}
	else if (Grade == Protocol::QUEST_NORMAL)
	{
		Img_QuestMark->ColorAndOpacity.R = 0.f;
		Img_QuestMark->ColorAndOpacity.G = 1.0f;
		Img_QuestMark->ColorAndOpacity.B = 0.f;
		Img_QuestMark->ColorAndOpacity.A = 1.0f;
	}
}

void UNpcInfoWidget::DrawTargetMark()
{
	RemoveQuestMark();
	RemoveCompletableMark();
	Img_TargetMark->SetVisibility(ESlateVisibility::Visible);
}

void UNpcInfoWidget::RemoveQuestMark()
{
	Img_QuestMark->SetVisibility(ESlateVisibility::Hidden);
}

void UNpcInfoWidget::RemoveCompletableMark()
{
	Img_CompletableMark->SetVisibility(ESlateVisibility::Hidden);
}

void UNpcInfoWidget::RemoveTargetMark()
{
	Img_TargetMark->SetVisibility(ESlateVisibility::Hidden);
}
