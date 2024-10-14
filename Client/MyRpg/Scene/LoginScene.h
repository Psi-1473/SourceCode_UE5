// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseScene.h"
#include "LoginScene.generated.h"

UCLASS()
class MYRPG_API ALoginScene : public ABaseScene
{
	GENERATED_BODY()
	
public:	
	ALoginScene();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
