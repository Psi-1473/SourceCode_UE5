// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/LoginScene.h"
#include "MyGameInstance.h"
#include "UIManager.h"

// Sets default values
ALoginScene::ALoginScene()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALoginScene::BeginPlay()
{
	Super::BeginPlay();
	//Cast<AMyPlayerController>
	TObjectPtr<APlayerController> PC = (GetWorld()->GetFirstPlayerController());
	PC->SetShowMouseCursor(true);
}

void ALoginScene::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	GInstance->GetUIMgr()->CloseUI(TEXT("CreateCharacter"));
	GInstance->GetUIMgr()->CloseUI(TEXT("CharacterSelecet"));
}


