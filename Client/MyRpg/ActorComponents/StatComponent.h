// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


DECLARE_DELEGATE_OneParam(FOnStatChanged, TObjectPtr<class UMyGameInstance>);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPG_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();
	~UStatComponent();

	void Init(TObjectPtr<class ACreature> Creature);

	void Refresh();
public:
	Protocol::StatInfo* GetStatInfo() { return StatInfo; }
	float GetHpRatio();
	float GetMpRatio();
	float GetExpRatio();
	
	struct FBaseStatData* GetStatData();

	void SetStatInfo(const Protocol::StatInfo& NewStatInfo);

public:
	FOnStatChanged StatChanged;
private:
	UPROPERTY()
	TWeakObjectPtr<class ACreature> OwnerCreature;
	Protocol::StatInfo* StatInfo;
		
};
