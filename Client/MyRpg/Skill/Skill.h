// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataStruct.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "Engine/DataAsset.h"
#include "Skill.generated.h"

/**
 * 
 */

UCLASS()
class MYRPG_API USkill : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	void Init(FSkillInfoData* Info, FRichImageRow* Img);
	// 히트 이펙트는 정보로 가지고 있다가 

	void UseSkill(TObjectPtr<class ARpgPlayer> Player);
	void SetQuickSlot(Protocol::EQuickSlot Type) { QuickType = Type; }
public:
	FSkillInfoData* GetSkillInfo() { return SkillInfo; }
	FRichImageRow* GetSkillImg() { return SkillImg; }
	FSlateBrush& GetSkillBrush() { return SkillImg->Brush; }
	Protocol::EQuickSlot GetQuickSlotKey() { return QuickType; }
private:
	FSkillInfoData* SkillInfo;
	FRichImageRow* SkillImg;
	Protocol::EQuickSlot QuickType;
};
