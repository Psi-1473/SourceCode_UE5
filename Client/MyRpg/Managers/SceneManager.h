// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataEnum.h"
#include "UObject/NoExportTypes.h"
#include "SceneManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USceneManager : public UObject
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);

	void LoadSceneAsync(ESceneType Type);

public:
	UFUNCTION()
	void OnLoadedScene();
	


private:
	ESceneType CurrentType = ESceneType::START;
	
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
	
};
