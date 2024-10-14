// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill.h"
#include "MyPlayer.h"

void USkill::Init(FSkillInfoData* Info, FRichImageRow* Img)
{
	SkillInfo = Info;
	SkillImg = Img;
}

void USkill::UseSkill(TObjectPtr<ARpgPlayer> Player)
{
	Player->UseSkill(SkillInfo->SkillId);

}
