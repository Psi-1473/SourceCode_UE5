// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "CreatureAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TextRender/DamageText.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "Components/WidgetComponent.h"
#include "Component/HpBarWidget.h"
#include "ActorComponents/StatComponent.h"
// Sets default values
#include "MyPlayer.h"
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DestPos = new Protocol::ObjectInfo();
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}


void AMonster::Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name)
{
	Super::Init(ObjInfo, StatInfo, Name);
	SetBaseStatInfo();
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	DestPos->set_x(GetActorLocation().X);
	DestPos->set_y(GetActorLocation().Y);
	DestPos->set_z(GetActorLocation().Z);
	
	HpBar->SetVisibility(false);
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDead(DeltaTime);
	MoveOnTick(DeltaTime);


}

/*
	************* public Functions ***************
*/

void AMonster::MoveOnTick(float DeltaTime)
{
	if(ObjectInfo == nullptr)
	{
		return;
	}
	if (ObjectInfo->state() != Protocol::STATE_IDLE &&
		ObjectInfo->state() != Protocol::STATE_MOVE &&
		ObjectInfo->state() != Protocol::STATE_RUN)
		return;

	const Protocol::EState State = DestPos->state();

	FVector DestPosition(DestPos->x(), DestPos->y(), DestPos->z());
	float Dist = FVector::Dist(DestPosition, GetActorLocation());

	// Tick에 Dist 비교하는거 추가한거 잘되나 확인하기
	if (State == Protocol::STATE_RUN || State == Protocol::STATE_MOVE)
	{
		if (Dist <= 91)
		{
			SetActorLocation(GetActorLocation());
			return;
		}
		const FRotator Rotation = GetActorRotation();
		FVector MoveDir = DestPosition - GetActorLocation();
		MoveDir.Normalize();
		FRotator TargetRotation = FRotator(0.0f, FRotationMatrix::MakeFromX(MoveDir).Rotator().Yaw, 0.0f);
		SetActorRotation(TargetRotation);
		AddMovementInput(MoveDir);
		FVector Start = GetActorLocation(); // 몬스터의 현재 위치
		FVector End = DestPosition; // 목표 위치
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 1.0f);

	}
	else if (State == Protocol::STATE_IDLE)
	{
		//FVector NewLocation = FMath::Lerp(GetActorLocation(), FVector(DestPosition.X, DestPosition.Y, DestPosition.Z + 88), DeltaTime * 5.f);
		SetActorLocation(GetActorLocation());
	}
}

void AMonster::Attack(FRotator Rotator)
{
	FVector DestPosition(DestPos->x(), DestPos->y(), DestPos->z());
	float Dist = FVector::Dist(DestPosition, GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Monster Attack (Dist = %f)"), Dist);
	
	SetActorRotation(Rotator);
	for (const auto& Inst : AnimInsts)
	{
		Inst->PlayAttackMontage(1);
	}
}

void AMonster::OnDamaged(float Damage, uint64 AttackerId)
{
	Super::OnDamaged(Damage, AttackerId);
	for (const auto& Inst : AnimInsts)
	{
		Inst->PlayDamagedMontage();
	}
	
	uint64 PlayerId = Cast<UMyGameInstance>(GetGameInstance())->GetPlayer()->GetObjectInfo()->object_id();
	if(AttackerId == PlayerId)
	{
		HpBar->SetVisibility(true);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		auto DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass,
			GetActorLocation() + DamageTextComp->GetRelativeLocation(),
			DamageTextComp->GetRelativeRotation(),
			SpawnParams);

		DamageText->SetDamageText(Damage);
	}
}



void AMonster::SetState(Protocol::EState State)
{
	Super::SetState(State);
	SetSpeed();

	auto MyPlayer = Cast<AActor>(Cast<UMyGameInstance>(GetGameInstance())->GetPlayer());
	if (GetDistanceTo(MyPlayer) > 1000.f)
	{
		HpBar->SetVisibility(false);
	}
}


/*
	************* Private Functions ***************
*/
void AMonster::SetBaseStatInfo()
{
	FString BlueprintClassName = GetClass()->GetName();
	FString IndexString = BlueprintClassName.Replace(TEXT("BP_Monster_"), TEXT(""));
	int32 Id = FCString::Atoi(*IndexString);

	BaseStat = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->GetDataMgr()->GetMonsterStatData(Id);
	TemplateId = Id;

	Hp = BaseStat->MaxHp;
	Cast<UHpBarWidget>(HpBar->GetWidget())->SetName(BaseStat->mobName);
	RefreshHpBar();
}

void AMonster::UpdateDead(float DeltaTime)
{
	if (ObjectInfo == nullptr)
	{
		return;
	}

	if (ObjectInfo->state() != Protocol::STATE_DEAD)
	{
		return;
	}

	DeadTick -= DeltaTime;

	if (DeadTick <= 0)
	{
		Destroy();
	}
}

void AMonster::SetSpeed()
{
	switch (ObjectInfo->state())
	{
	case Protocol::STATE_MOVE:
		GetCharacterMovement()->MaxWalkSpeed = 170.f;
		break;
	case Protocol::STATE_RUN:
		GetCharacterMovement()->MaxWalkSpeed = 420.f;
		break;
	default:
		break;
	}
}

