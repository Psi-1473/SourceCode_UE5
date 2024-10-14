// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageText.generated.h"

UCLASS()
class MYRPG_API ADamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageText();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void InitSettingValue();
	void SetDamageText(float Damage);
private:


private:
	UPROPERTY()
	TWeakObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	class UTextRenderComponent* TextRender;

	UPROPERTY()
	FTimerHandle DestroyTimerHandle;
};
