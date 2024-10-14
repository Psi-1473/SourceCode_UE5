// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Protocol.pb.h"
#include "SkillImgWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USkillImgWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(int32 Id, TObjectPtr<class UMyGameInstance> Instance);

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DragVisualClass;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UImage> Img_Skill;;

	UPROPERTY()
	TObjectPtr<class USkill> Skill;

	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;
};
