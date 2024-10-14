// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/BaseScene.h"
#include "StartScene.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API AStartScene : public ABaseScene
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
