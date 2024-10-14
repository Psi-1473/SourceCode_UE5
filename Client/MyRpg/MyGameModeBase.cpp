// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Scene/InGameMainWidget.h"
#include "UObject/ConstructorHelpers.h"

AMyGameModeBase::AMyGameModeBase()
{
	//static ConstructorHelpers::FClassFinder<UInGameMainWidget> Hud(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Scene/WBP_Main.WBP_Main_C'"));
	//if (Hud.Succeeded())
	//{
	//	HUD_Class = Hud.Class;
	//	CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
	//	if (CurrentWidget)
	//	{
	//		CurrentWidget->AddToViewport();
	//		//CurrentWidget->RemoveFromViewport();
	//
	//	}
	//}
}
