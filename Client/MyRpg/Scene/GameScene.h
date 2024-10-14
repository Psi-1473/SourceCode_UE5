// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/BaseScene.h"
#include "GameScene.generated.h"

/**
 * 
 */


UCLASS()
class MYRPG_API AGameScene : public ABaseScene
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
