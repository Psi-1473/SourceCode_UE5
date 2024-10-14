// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UResourceManager : public UObject
{
	GENERATED_BODY()


public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);


	TObjectPtr<AActor> InstantiateActor(FString Path, FString Name, FVector SpawnLocation);
private:
	UClass* LoadObjectClass(FString Path, FString Name);
	//TObjectPtr<class USoundCue> 
	//UClass* LoadObjectClassAsyn(FString Path, FString Name);

	//void InstantiateActorAsync(FString Path, FString Name);


private:
	//FStreamableManager StreamableManager;
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	const FString BasePath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/");
};
