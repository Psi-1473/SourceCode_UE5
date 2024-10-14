// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraShakeBase.h"
#include "MyAnimNotify_CameraShake.generated.h"
/**
 * 
 */
UCLASS()
class MYRPG_API UMyAnimNotify_CameraShake : public UAnimNotify
{
    GENERATED_BODY()

public:
    UMyAnimNotify_CameraShake();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
