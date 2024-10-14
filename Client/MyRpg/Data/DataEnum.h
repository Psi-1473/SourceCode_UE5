// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataEnum.generated.h"

/**
 * 
 */

enum class InteractUIType
{
	SHOP,
	QUEST,
	EXIT,
};

enum class ESceneType
{
	START,
	TITLE,
	LOGIN,
	GAME,
};
UCLASS()
class MYRPG_API UDataEnum : public UObject
{
	GENERATED_BODY()
	
};
