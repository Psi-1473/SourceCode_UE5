// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DataEnum.h"
#include "InteractListWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(On)
UCLASS()
class MYRPG_API UInteractListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(InteractUIType InitType, TWeakObjectPtr<class ANpc> Npc, TObjectPtr<class UMyGameInstance> Instance);
	// 나가기
	// 상점
	// 퀘스트
	UFUNCTION()
	void OnMenuClicked();

private:
	void SetQuestType();
	void SetShopType();
	void SetExitType();

	void PopupQuest();
	void PopupShop();
	void Exit();

	
private:
	InteractUIType Type;
	typedef void (UInteractListWidget::* FunctionPointerType)();
	FunctionPointerType OnButtonClicked;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Img_ItemBackground;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Click;
	
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TWeakObjectPtr<class ANpc> OwnerNpc;
};
