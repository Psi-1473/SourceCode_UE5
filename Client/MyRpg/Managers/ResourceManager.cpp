// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ResourceManager.h"
#include "MyGameInstance.h"
//#include "Engine/StreamableManager.h"



void UResourceManager::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
	
}

UClass* UResourceManager::LoadObjectClass(FString Path, FString Name)
{
	FString BP_Name = Name + TEXT(".") + Name + TEXT("'");
	FString DestPath = BasePath + Path + BP_Name;
	UClass* Class = StaticLoadClass(UObject::StaticClass(), nullptr, *DestPath);

	return Class;
}

TObjectPtr<AActor> UResourceManager::InstantiateActor(FString Path, FString Name, FVector SpawnLocation)
{
	//UClass* Class = LoadObjectClass(Path, Name);
	TSubclassOf<AActor> ActorClass = LoadObjectClass(Path, Name);
	FRotator RandomRotation = FRotator(0, FMath::RandRange(0, 360), 0);
	TObjectPtr<AActor> NewActor = (GInstance->GetWorld()->SpawnActor(ActorClass, &SpawnLocation));


	return NewActor;
}



//void UResourceManager::InstantiateActorAsync(FString Path, FString Name)
//{
//	// 액터를 씬에 소환 비동기
//}
