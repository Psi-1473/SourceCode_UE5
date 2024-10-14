// Fill out your copyright notice in the Description page of Project Settings.


#include "StartScene.h"
#include "MyGameInstance.h"
#include "SceneManager.h"

void AStartScene::BeginPlay()
{
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	// Temp �̰� ���߿� StartScene���� ����.
	//GInstance->Init();
	
	GInstance->GetSceneMgr()->LoadSceneAsync(ESceneType::LOGIN);

	
}
