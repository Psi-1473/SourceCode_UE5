// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/BaseScene.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"

// Sets default values
ABaseScene::ABaseScene()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseScene::BeginPlay()
{
	Super::BeginPlay();
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (SceneUIClass != nullptr)
	{
		GInstance->GetUIMgr()->SetMainWidget(SceneUIClass);
		
	}
}

