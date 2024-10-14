// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UDynamicInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Refresh() {}
};
