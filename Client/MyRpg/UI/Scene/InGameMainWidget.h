// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "SceneWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UInGameMainWidget : public USceneWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void Init(TObjectPtr<class UMyGameInstance> Instance) override;
	virtual void Refresh(TObjectPtr<class UMyGameInstance> Instance) override;

	//UInGameMainWidget(const FObjectInitializer& ObjectInitializer);

	

private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UHorizontalBox> Slots;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UHorizontalBox> Slots2;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class URichTextBlock> Text_Level;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_Class;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UProgressBar> Bar_Hp;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UProgressBar> Bar_Mp;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UProgressBar> Bar_Exp;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
