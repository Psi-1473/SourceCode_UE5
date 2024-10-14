// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/SkillSlotWidget.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "MyPlayer.h"
#include "Skill/Skill.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SubItem/SkillImgWidget.h"
#include "ActorComponents/SkillComponent.h"




void USkillSlotWidget::NativeConstruct()
{
	GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	SkillImg->Init(SkillId, GInstance);

	Refresh();

	Btn_LevelUp->OnClicked.AddDynamic(this, &USkillSlotWidget::UpgradeSkill);
	Handle = GInstance->GetPlayer()->GetSkillComponent()->OnSkillUpdated.AddUObject(this, &USkillSlotWidget::Refresh);
}

void USkillSlotWidget::NativeDestruct()
{
	GInstance->GetPlayer()->GetSkillComponent()->OnSkillUpdated.Remove(Handle);
}

void USkillSlotWidget::Refresh()
{
	Protocol::ECharacterType Type = GInstance->GetPlayer()->GetObjectInfo()->objtype();
	TObjectPtr<USkill> Skill = GInstance->GetDataMgr()->GetSkill(Type, SkillId);
	int32 SkillLevel = GInstance->GetPlayer()->GetSkillComponent()->GetSkillLevel(SkillId);
	int32 SkillPoint = GInstance->GetPlayer()->GetSkillComponent()->GetSkillPoint();

	if (SkillPoint <= 0)
	{
		Btn_LevelUp->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Btn_LevelUp->SetVisibility(ESlateVisibility::Visible);
	}
	Text_SkillName->SetText(FText::FromString(Skill->GetSkillInfo()->SkillName));
	Text_Level->SetText(FText::FromString(FString::FromInt(SkillLevel)));


}

void USkillSlotWidget::UpgradeSkill()
{
	GInstance->GetPlayer()->GetSkillComponent()->SendUpgradeSkillPacket(SkillId);
}

