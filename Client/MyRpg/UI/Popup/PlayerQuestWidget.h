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

	// Ư�� ����Ʈ�� ���� �����ϴ� ���� ���� : Ư�� ����Ʈ ��� Ŭ���� ����
	void SetDescWidget(int32 QuestId);
	virtual void Refresh() override;;

private:
	
	// : ����Ʈ ����� ���� �⺻���� ���� ���� (����Ʈ �̸�, ���� ��)
	void OpenAndSetQuestDescWidget(int32 QuestId);

	// ����Ʈ ���� ������ ������ "����Ʈ Ÿ�� ����" ����
	void RemoveTargetsFromViewport();

	// ����Ʈ ���� ������ "����Ʈ Ÿ�� ����" ����
	void SetTargetInfoWithDescWidget(int32 QuestId);

	// ���� ���� ���� ����Ʈ ������ �ҷ��� Widget�� ����.
	void SetInProgressQuests();



private:
	UPROPERTY(meta=(bindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_QuestList;

	UPROPERTY(meta = (bindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	// ======= ����Ʈ ���� UI ======
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
