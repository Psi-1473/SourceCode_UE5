// Fill out your copyright notice in the Description page of Project Settings.


#include "TextRender/DamageText.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "Camera/CameraComponent.h"

ADamageText::ADamageText()
{
	PrimaryActorTick.bCanEverTick = true;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXTRENDER"));
	TextRender->SetXScale(3.f);
	TextRender->SetYScale(3.f);
	InitSettingValue();
	RootComponent = TextRender;

}

void ADamageText::BeginPlay()
{
	Super::BeginPlay();
	FTimerDelegate TimeCallback;
	UE_LOG(LogTemp, Warning, TEXT("DamageText!"));
	TimeCallback.BindLambda(
		[this] {this->Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
		});
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, TimeCallback, 0.5f, false);

	auto Char = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Camera = Cast<AMyPlayer>(Char)->GetFollowCamera();

	
}

void ADamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 쳐다보게 하기

	if (Camera == nullptr)
		return;


	//1. 바라볼 방향 정하기
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Camera->GetComponentLocation());
	// 2. 바라보게 하기
	SetActorRotation(Rot);
	FVector NextPos = GetActorLocation() + FVector(0.f, 1.f, 2.f);
	SetActorLocation(NextPos);

}

void ADamageText::InitSettingValue()
{
	TextRender->SetTextRenderColor(FColor(255.f, 0.f, 0.f, 255.f));
}

void ADamageText::SetDamageText(float Damage)
{
	FString Text = FString::FromInt(Damage);
	TextRender->SetText(FText(FText::FromString(Text)));
}

