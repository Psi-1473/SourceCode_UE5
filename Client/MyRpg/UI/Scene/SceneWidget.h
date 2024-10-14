// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SceneWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USceneWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init(TObjectPtr<class UMyGameInstance> Instance);
	virtual void Refresh(TObjectPtr<class UMyGameInstance> Instance);
};
