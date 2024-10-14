// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CreatureAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UCreatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage(int AttackSequence);
	void PlayDamagedMontage();


public:
	void SetDead(bool Value) { bDead = Value; }
protected:
	void PlayMontage(UAnimMontage* Montage, FName SectionName, float Rate = 1.0f);

	void InitializeOwner();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DamagedMontage;


protected:
	UPROPERTY()
	TObjectPtr<class ACreature> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bDead;
};
