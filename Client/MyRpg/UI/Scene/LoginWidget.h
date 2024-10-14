// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Scene/SceneWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API ULoginWidget : public USceneWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Login();

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UEditableText> EText_Id;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UEditableText> EText_Pw;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Login;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_CreateId;
};
