// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "NpcInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UNpcInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(struct FNpcData* NpcData);
	void DrawQuestMark(Protocol::EQuestGrade Grade);
	void DrawCompletableMark(Protocol::EQuestGrade Grade);
	void DrawTargetMark();


	void RemoveQuestMark();
	void RemoveCompletableMark();
	void RemoveTargetMark();


private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UTextBlock> Text_Title;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_QuestMark;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_CompletableMark;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UImage> Img_TargetMark;
};
