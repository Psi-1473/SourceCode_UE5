// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlateBasics.h"
#include "SkillSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void UpgradeSkill();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	int32 SkillId;
private:
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class USkillImgWidget> SkillImg;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UTextBlock> Text_SkillName;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UTextBlock> Text_Level;

	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UButton> Btn_LevelUp;


	UPROPERTY()
	TObjectPtr<class UMyGameInstance> GInstance;

	UPROPERTY()
	bool BorderClicked = false;

	FDelegateHandle Handle;
	
};
