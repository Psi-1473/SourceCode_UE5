// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Refresh() {}
};
