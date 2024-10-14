// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "Data/DataStruct.h"
#include "NpcWidget.generated.h"

/**
 * 
 */
class ANpc;
enum class InteractUIType;
UCLASS()
class MYRPG_API UNpcWidget : public UPopupWidget
{
	GENERATED_BODY()
	
public:
	UNpcWidget(const FObjectInitializer& ObjectInitializer);
	void Init(TObjectPtr<ANpc> NewNpc, TObjectPtr<class UMyGameInstance> GInstance);
	void SetDialogueMode(int32 QuestId, int32 Type);
	void SetGeneralMode();

	UFUNCTION()
	void ShowNextDialogue();

private:
	void SetBaseInfo();
	void SetInteractInfo(InteractUIType InteractType);
	void SetDialogueInfo(int32 QuestId, int32 Type);
private:
	UPROPERTY()
	TSubclassOf<class UUserWidget> InteractListClass;

	UPROPERTY()
	TWeakObjectPtr<ANpc> OwnerNpc;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> Text_Line;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Img_NpcType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> InteractList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_LineNext;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	TSharedPtr<struct FQuestDialogue> Dialogue;
	int32 DialogueIndex = -1;
	int32 DialogueType;
	int32 DialogueQuestId;

};
