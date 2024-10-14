// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "Blueprint/UserWidget.h"
#include "SkillInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USkillInfoWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	// 플레이어 정보, 스킬 정보 받아서 스킬 정보 초기화
	void Init(TObjectPtr<class USkill> Skill);

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UImage> Img_Skill;
	
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> RText_SkillName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> RText_SkillDesc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> RText_SkillType;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Mp;

};
