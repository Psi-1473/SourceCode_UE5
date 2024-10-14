// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureAnimInstance.h"
#include "Protocol.pb.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UPlayerAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()


public:
	UPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	void PlaySkillAnim(int32 SkillId);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Vertical;
};
