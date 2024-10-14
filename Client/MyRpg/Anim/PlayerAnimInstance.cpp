// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/PlayerAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "RpgPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimMontage.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Animation/Montage/Player/Warrior/AM_Warrior_Skill.AM_Warrior_Skill'"));

	if (Skill.Succeeded())
	{
		SkillMontage = Skill.Object;
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Character = Cast<ARpgPlayer>(Owner);
	if(Character == nullptr)
	{
		return;
	}
	Horizontal = Character->GetHorizontal();
	Vertical = Character->GetVertical();
}


void UPlayerAnimInstance::PlaySkillAnim(int32 SkillId)
{
	FName SectionName(*FString::FromInt(SkillId));
	PlayMontage(SkillMontage, SectionName);
}

