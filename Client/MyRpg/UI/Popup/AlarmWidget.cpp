// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/AlarmWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyGameInstance.h"
#include "UIManager.h"

void UAlarmWidget::NativeConstruct()
{
	GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	Btn_Confirm->OnClicked.AddDynamic(this, &UAlarmWidget::CloseThisUI);
}

void UAlarmWidget::SetText(FString Text)
{
	Text_Alarm->SetText(FText::FromString(Text));
}

void UAlarmWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("Alarm"));
}
