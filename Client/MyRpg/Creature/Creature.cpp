// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Creature.h"
#include "CreatureAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Component/HpBarWidget.h"

// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageTextComp = CreateDefaultSubobject<USceneComponent>(TEXT("DamageTextComponent"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetDrawSize(FVector2D(60.f, 5.f));
	HpBar->SetupAttachment(RootComponent);
	

}
void ACreature::Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name)
{
	SetObjectInfo(ObjInfo);

	StatComponent->Init(this);
	StatComponent->SetStatInfo(StatInfo);

	SetState(Protocol::STATE_IDLE);
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);
	//GetComponentByClass<USkeletalMeshComponent>
	for (int i = 0; i < MeshComponents.Num(); i++)
	{
		AnimInsts.Add(Cast<UCreatureAnimInstance>(MeshComponents[i]->GetAnimInstance()));
	}
	
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACreature::SetState(Protocol::EState State)
{
	if (ObjectInfo->state() == State)
		return;

	if (State == Protocol::STATE_IDLE)
	{
		bUseControllerRotationYaw = false;
	}
	else
	{
		bUseControllerRotationYaw = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("IDLE SET"));
	ObjectInfo->set_state(State);

}

void ACreature::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	if (ObjectInfo == nullptr)
	{
		ObjectInfo = new Protocol::ObjectInfo();
	}
	ObjectInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void ACreature::SetDestInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	// Dest에 최종 상태 복사.
	DestPos->CopyFrom(Info);

	// 상태만 바로 적용하자.
	SetState(Info.state());
}

void ACreature::OnDamaged(float Damage, uint64 AttackerId)
{
	if(GetState() != Protocol::STATE_SKILL)
	{
		SetState(Protocol::STATE_DAMAGED);
	}
}

void ACreature::Die()
{
	SetState(Protocol::STATE_DEAD);
	for (const auto& Inst : AnimInsts)
	{
		Inst->SetDead(true);
	}
}

void ACreature::OnStatChanged(const Protocol::StatInfo& Info)
{
	UE_LOG(LogTemp, Warning, TEXT("STAT CHANGED"));
	StatComponent->SetStatInfo(Info);
	RefreshHpBar();
}

void ACreature::RefreshHpBar()
{
	Cast<UHpBarWidget>(HpBar->GetWidget())->SetPercent(StatComponent->GetHpRatio());
}


