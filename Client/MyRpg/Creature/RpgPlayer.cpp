// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/RpgPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "MyPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Component/HpBarWidget.h"

#include "Component/HpBarWidget.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "ResourceManager.h"
// Sets default values
ARpgPlayer::ARpgPlayer()
{

 	//// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	ObjectInfo = new Protocol::ObjectInfo();
	DestPos = new Protocol::ObjectInfo();
	MoveInfo = new Protocol::PlayerMoveInfo();
	AttackSequence = 0;
	MaxAttackSequence = 3; //temp
	bCanAttack = true;
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	WarriorWeaponSocket = FName("Socket_Sword_L");
	WizardWeaponSocket = FName("Socket_Staff_L");
	RogueWeaponSocketL = FName("Socket_Dagger_L");
	RogueWeaponSocketR = FName("Socket_Dagger_R");
}

ARpgPlayer::~ARpgPlayer()
{
	delete ObjectInfo;
	delete DestPos;
	ObjectInfo = nullptr;
	DestPos = nullptr;
}

void ARpgPlayer::Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name)
{
	Super::Init(ObjInfo, StatInfo, Name);
	SetAnimClass(ObjectInfo->objtype());
	SetWeapon(ObjectInfo->objtype());
	PlayerName = Name;
	Cast<UHpBarWidget>(HpBar->GetWidget())->SetName(PlayerName);
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);
	//GetComponentByClass<USkeletalMeshComponent>
	for (int i = 0; i < MeshComponents.Num(); i++)
	{
		AnimInsts.Add(Cast<UCreatureAnimInstance>(MeshComponents[i]->GetAnimInstance()));
	}
}

// Called when the game starts or when spawned
void ARpgPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	{
		FVector Location = GetActorLocation();
		DestPos->set_x(Location.X);
		DestPos->set_y(Location.Y);
		DestPos->set_z(Location.Z);
		MoveInfo->set_yaw(GetControlRotation().Yaw);

		SetState(Protocol::STATE_IDLE);
	}
	
}

// Called every frame
void ARpgPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FVector Location = GetActorLocation();
		ObjectInfo->set_x(Location.X);
		ObjectInfo->set_y(Location.Y);
		ObjectInfo->set_z(Location.Z);
	}

	MoveOnTick(DeltaTime);
}



void ARpgPlayer::MoveOnTick(float DeltaTime)
{
	if (IsMyPlayer() == false)
	{
		const Protocol::EState State = ObjectInfo->state();


		if (State == Protocol::STATE_RUN)
		{
			const FRotator Rotation = GetActorRotation();
			const FRotator YawRotation(0, MoveInfo->yaw(), 0);

			// get forward vector
			FVector DesireDir = FVector(MoveInfo->dirx(), MoveInfo->diry(), MoveInfo->dirz());
			SetActorRotation(YawRotation);

			AddMovementInput(DesireDir);
		}
		else
		{
			SynchronizePosition();
		}
	}
}

void ARpgPlayer::Attack(FRotator Rotation)
{
	// 공격 애니메이션 실행
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
	SetState(Protocol::STATE_ATTACK);
	if(!IsMyPlayer())
	{
		SetActorRotation(Rotation);
	}

	for (const auto& Inst : AnimInsts)
	{
		Inst->PlayAttackMontage(AttackSequence);
	}
	
}

void ARpgPlayer::UseSkill(int32 SkillId)
{
	SetState(Protocol::STATE_SKILL);

	for (const auto& Inst : AnimInsts)
	{
		Cast<UPlayerAnimInstance>(Inst)->PlaySkillAnim(SkillId);
	}
	//Cast<UPlayerAnimInstance>(AnimInst)->PlaySkillAnim(SkillId);
}

void ARpgPlayer::OnDamaged(float Damage, uint64 AttackerId)
{
	if(GetState() != Protocol::STATE_SKILL)
	{	
		for (const auto& Inst : AnimInsts)
		{
			Inst->PlayDamagedMontage();
		}
	}
}

bool ARpgPlayer::IsMyPlayer()
{
	return Cast<AMyPlayer>(this) != nullptr;
}

/*
		Private Functions
*/
void ARpgPlayer::SetHpBarVisibility(bool Value)
{
	HpBar->SetVisibility(Value);
}
void ARpgPlayer::SynchronizePosition()
{
	FVector Dest = FVector(DestPos->x(), DestPos->y(), DestPos->z());
	float InterpAmount = 0.1f;

	// 현재 위치에서 목표 위치까지의 벡터를 계산합니다.
	FVector Direction = Dest - GetActorLocation();
	float Distance = Direction.Size();

	if (Distance < KINDA_SMALL_NUMBER)
	{
		SetActorLocation(Dest);
		return;
	}

	Direction.Normalize();

	// 새로운 위치를 계산합니다.
	FVector NewPosition = GetActorLocation() + Direction * InterpAmount;

	// 새로운 위치를 설정합니다.
	SetActorLocation(NewPosition);
}

void ARpgPlayer::SetAnimClass(Protocol::ECharacterType Type)
{	
	FString Dir;
	switch (Type)
	{
	case Protocol::WARRIOR:
		Dir = TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Animation/Creature/Player/ABP_Warrior.ABP_Warrior_C'");
		break;
	case Protocol::ROGUE:
		Dir = TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Animation/Creature/Player/ABP_Rogue.ABP_Rogue_C'");
		break;
	case Protocol::WIZARD:
		Dir = TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Animation/Creature/Player/ABP_Wizard.ABP_Wizard_C'");
		break;
	default:
		break;
	}

	auto AnimClass = LoadClass<UAnimInstance>(NULL, *Dir, NULL, LOAD_None, NULL);
	GetMesh()->SetAnimClass(AnimClass);
}

void ARpgPlayer::SetWeapon(Protocol::ECharacterType Type)
{
	FString Dir;
	TObjectPtr<AActor> Weapon = nullptr;
	FString Name = TEXT("");
	switch (Type)
	{
	case Protocol::WARRIOR:
		Name = TEXT("BP_BaseSword.BP_BaseSword_C");
		Weapon = Cast<UMyGameInstance>(GetGameInstance())->GetResourceMgr()->InstantiateActor(TEXT("Weapon/"), Name, FVector(0.f, 0.f, 1200.f));
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WarriorWeaponSocket);
		UE_LOG(LogTemp, Warning, TEXT("WARRIOR WEAPON !"));
		break;
	case Protocol::ROGUE:
		Name = TEXT("BP_BaseDagger.BP_BaseDagger_C");
		Weapon = Cast<UMyGameInstance>(GetGameInstance())->GetResourceMgr()->InstantiateActor(TEXT("Weapon/"), Name, FVector(0.f, 0.f, 1200.f));
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RogueWeaponSocketL);
		Weapon = Cast<UMyGameInstance>(GetGameInstance())->GetResourceMgr()->InstantiateActor(TEXT("Weapon/"), Name, FVector(0.f, 0.f, 1200.f));
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RogueWeaponSocketR);

		break;
	case Protocol::WIZARD:
		Name = TEXT("BP_BaseStaff.BP_BaseStaff_C");
		Weapon = Cast<UMyGameInstance>(GetGameInstance())->GetResourceMgr()->InstantiateActor(TEXT("Weapon/"), Name, FVector(0.f, 0.f, 1200.f));
		if(Weapon == nullptr)
			break;
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WizardWeaponSocket);
		UE_LOG(LogTemp, Warning, TEXT("WIZARD WEAPON !"));
		break;
		break;
	default:
		break;
	}
}





