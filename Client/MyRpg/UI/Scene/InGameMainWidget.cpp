// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Scene/InGameMainWidget.h"
#include "SubItem/QuickSlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "UIManager.h"
#include "MyPlayer.h"
#include "ActorComponents/StatComponent.h"
#include "MyGameInstance.h"
#include "DataManager.h"




void UInGameMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i <  Slots->GetChildrenCount(); i++)
	{
		Cast<UQuickSlotWidget>(Slots->GetChildAt(i))->SetKey();
	}

	for (int i = 0; i < Slots2->GetChildrenCount(); i++)
	{
		Cast<UQuickSlotWidget>(Slots2->GetChildAt(i))->SetKey();
	}

	GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	//GInstance->GetUIMgr()->SetMainWidget(this);

}

void UInGameMainWidget::Init(TObjectPtr<UMyGameInstance> Instance)
{	
	auto StatComp = Instance->GetPlayer()->GetStatComponent();
	UE_LOG(LogTemp, Warning, TEXT("Main Widget Init"));
	StatComp->StatChanged.BindUObject(this, &UInGameMainWidget::Refresh);
}

void UInGameMainWidget::Refresh(TObjectPtr<UMyGameInstance> Instance)
{
	auto StatComp = Instance->GetPlayer()->GetStatComponent();
	Text_Level->SetText(FText::FromString(FString::FromInt(StatComp->GetStatInfo()->level())));
	Bar_Hp->SetPercent(StatComp->GetHpRatio());
	Bar_Mp->SetPercent(StatComp->GetMpRatio());
	Bar_Exp->SetPercent(StatComp->GetExpRatio());
}
