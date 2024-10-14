// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAnimInstance.h"
#include "Creature.h"

void UCreatureAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	InitializeOwner();
}

void UCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Owner == nullptr)
	{
		return;
	}
	Speed = Owner->GetVelocity().Size();
}

void UCreatureAnimInstance::PlayAttackMontage(int AttackSequence)
{
	FString SectionString = TEXT("Attack") + FString::FromInt(AttackSequence);
	FName SectionName(*SectionString);

	PlayMontage(AttackMontage, SectionName);
}

void UCreatureAnimInstance::PlayDamagedMontage()
{
	Montage_Stop(1.0f);
	Montage_Play(DamagedMontage);
}

void UCreatureAnimInstance::PlayMontage(UAnimMontage* Montage, FName SectionName, float Rate)
{
	Montage_Stop(1.0f);
	Montage_Play(Montage, Rate);
	Montage_JumpToSection(SectionName, Montage);
}

void UCreatureAnimInstance::InitializeOwner()
{
	auto Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn))
	{
		return;
	}

	auto Character = Cast<ACreature>(Pawn);
	if (!Character)
	{
		return;
	}

	Owner = Character;
	bDead = false;
}
