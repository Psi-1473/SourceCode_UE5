// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/MyAnimNotify_CameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UMyAnimNotify_CameraShake::UMyAnimNotify_CameraShake()
{
}

void UMyAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && CameraShakeClass)
    {
        AActor* Owner = MeshComp->GetOwner();
        if (Owner)
        {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0);
            if (PlayerController)
            {
                UE_LOG(LogTemp, Warning, TEXT("Camera Shake"));
                PlayerController->ClientStartCameraShake(CameraShakeClass, 1.0f);
            }
        }
    }
}
