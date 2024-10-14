// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseScene.generated.h"

UCLASS()
class MYRPG_API ABaseScene : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseScene();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SceneUIClass;
	
};
