// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataEnum.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USoundManager : public UObject
{
	GENERATED_BODY()

public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);
	void PlayBgm(ESceneType SceneType);
	void StopBgm();
	void PlayEffectSound(FString Path);

	UFUNCTION()
	void OnBgmLoaded();

	UFUNCTION()
	void OnBgmFinished();

private:
	UPROPERTY()
	TObjectPtr<class USoundBase> Bgm;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	FString BgmPath;

};
