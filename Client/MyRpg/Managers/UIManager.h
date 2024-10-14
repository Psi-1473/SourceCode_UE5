// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UUIManager : public UObject
{
	GENERATED_BODY()
public:
	UUIManager();

	void Init(TObjectPtr<class UMyGameInstance> Instance);
	void InitializeSceneWidget();
	void Alarm(FString Text);
public:
	void SetMainWidget(TSubclassOf<class UUserWidget> WidgetClass);
	TObjectPtr<class UUserWidget> PopupUI(FString Type);
	TObjectPtr<class UUserWidget> PopupDynamicInfoUI(FString Type);
	TObjectPtr<class UUserWidget> CreateSubItem(TSubclassOf<class UUserWidget> SubItem);


	void RefreshUI(FString UiType);
	void CloseUI(FString Type);
	void CloseAllUI();

	//temp
	TObjectPtr<class USceneWidget> GetSceneWidget() { return SceneWidget; }

public:
	TObjectPtr<class UPopupWidget> GetPopupUI(FString Type);
	TObjectPtr<class UUserWidget> GetDynamicUI(FString Type);
private:
	void SetPlayerMouseCursor(bool Value);
private:
	UPROPERTY()
	TWeakObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	TMap<FString, TObjectPtr<class UPopupWidget>> PopupUIList;

	UPROPERTY()
	TMap<FString, TObjectPtr<class UDynamicInfoWidget>> DynamicInfoUIList;

	UPROPERTY()
	TObjectPtr<class USceneWidget> SceneWidget;
};
