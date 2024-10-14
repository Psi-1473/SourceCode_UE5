// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/SkillInfoWidget.h"
#include "Skill/Skill.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/Image.h"

void USkillInfoWidget::Init(TObjectPtr<class USkill> Skill)
{
	Img_Skill->SetBrush(Skill->GetSkillBrush());
	RText_SkillName->SetText(FText::FromString(Skill->GetSkillInfo()->SkillName));
	RText_SkillDesc->SetText(FText::FromString(Skill->GetSkillInfo()->SkillDesc));

	//RText_SkillType, Text_Mp 변경하기
}
