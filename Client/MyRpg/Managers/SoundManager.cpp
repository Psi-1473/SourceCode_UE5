// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SoundManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MyGameInstance.h"

void USoundManager::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
}

void USoundManager::PlayBgm(ESceneType SceneType)
{
	int32 BgmIndex = static_cast<int32>(SceneType);
	BgmPath = FString::Printf(TEXT("/Game/Sounds/Bgm/Bgm_%d.Bgm_%d"), BgmIndex, BgmIndex);
	FSoftObjectPath SoundCuePath(*BgmPath);
	UE_LOG(LogTemp, Warning, TEXT("Load! Sound: %s"), *BgmPath);
	if (SoundCuePath.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound path is null: %s"), *BgmPath);
		return;
	}
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(SoundCuePath, FStreamableDelegate::CreateUObject(this, &USoundManager::OnBgmLoaded));

}

void USoundManager::StopBgm()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr; // √ ±‚»≠
	}
}

void USoundManager::PlayEffectSound(FString Path)
{
}

void USoundManager::OnBgmLoaded()
{
	//UE_LOG(LogTemp, Warning, TEXT("On Bgm Loaded"));
	//StopBgm();
	//FSoftObjectPath SoundCuePath(*BgmPath);
	//TSoftObjectPtr<USoundWave> SoundCuePtr(SoundCuePath);
	//
	//if (SoundCuePtr.Get() == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Bgm Null"), );
	//}
	//AudioComponent = UGameplayStatics::SpawnSound2D(GInstance->GetWorld(), SoundCuePtr.Get());
	//if (AudioComponent)
	//{
	//	AudioComponent->OnAudioFinished.AddDynamic(this, &USoundManager::OnBgmFinished);
	//}
	
}

void USoundManager::OnBgmFinished()
{
	OnBgmLoaded();
}
