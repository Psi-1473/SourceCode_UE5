// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHpBarWidget::SetName(FString Name)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set Name : %s"), *Name);
	FText Text = FText::FromString(Name);

	if (Text_Name == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Text_Name Nullptr"));
		return;
	}
	Text_Name->SetText(Text);
}

void UHpBarWidget::SetPercent(float Ratio)
{
	Bar_Hp->SetPercent(Ratio);
	UE_LOG(LogTemp, Warning, TEXT("Hp Ratio : %f"), Ratio);
}
