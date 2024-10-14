// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene.h"
#include "Protocol.pb.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "UIManager.h"
#include "MyPlayerController.h"


void AGameScene::BeginPlay()
{ 
	Super::BeginPlay();
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	Protocol::C_ENTER_GAME Pkt;
	
	Pkt.set_playerid(GInstance->CharacterDBId);
	
	SEND_PACKET(Pkt);

	GInstance->GetDataMgr()->SetSkillInfo();
	
}

