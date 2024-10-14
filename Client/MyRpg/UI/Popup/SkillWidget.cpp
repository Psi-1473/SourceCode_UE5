// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/SkillWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ActorComponents/SkillComponent.h"
#include "MyPlayer.h"



void USkillWidget::Init(TObjectPtr<UMyGameInstance> Instance)
{
	GInstance = Instance;
	Btn_Exit->OnClicked.AddDynamic(this, &USkillWidget::CloseThisUI);
	Handle = GInstance->GetPlayer()->GetSkillComponent()->OnSkillUpdated.AddUObject(this, &USkillWidget::Refresh);
	Refresh();
}

void USkillWidget::NativeDestruct()
{
	GInstance->GetPlayer()->GetSkillComponent()->OnSkillUpdated.Remove(Handle);
}

void USkillWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("Skill"));
}

void USkillWidget::Refresh()
{
	int32 SkillPoint = GInstance->GetPlayer()->GetSkillComponent()->GetSkillPoint();
	Text_Point->SetText(FText::FromString(FString::FromInt(SkillPoint)));
}
