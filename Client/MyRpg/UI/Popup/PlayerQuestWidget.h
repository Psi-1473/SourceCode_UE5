// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Popup/PopupWidget.h"
#include "PlayerQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UPlayerQuestWidget : public UPopupWidget
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr<class UMyGameInstance> Instance);
	
	UFUNCTION()
	void CloseThisUI();
	UFUNCTION()
	void HideDescUI();
	UFUNCTION()
	void RegisterQuickQuest();

	// 특정 퀘스트에 대해 설명하는 위젯 띄우기 : 특정 퀘스트 목록 클릭시 실행
	void SetDescWidget(int32 QuestId);
	virtual void Refresh() override;;

private:
	
	// : 퀘스트 목록을 열고 기본적인 정보 세팅 (퀘스트 이름, 설명 등)
	void OpenAndSetQuestDescWidget(int32 QuestId);

	// 퀘스트 설명 위젯에 설정된 "퀘스트 타겟 정보" 제거
	void RemoveTargetsFromViewport();

	// 퀘스트 설명 위젯에 "퀘스트 타겟 정보" 세팅
	void SetTargetInfoWithDescWidget(int32 QuestId);

	// 현재 진행 중인 퀘스트 정보를 불러와 Widget에 띄운다.
	void SetInProgressQuests();



private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_QuestList;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	// ======= 퀘스트 설명 UI ======
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UCanvasPanel> Desc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_HideDesc;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_RegisterQuick;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_QuestName;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UTextBlock> Text_QuestDesc;
	
	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UVerticalBox> TargetBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> QuestSlotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> TargetSlotClass;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	struct FQuestData* SelectedQuest;
};
