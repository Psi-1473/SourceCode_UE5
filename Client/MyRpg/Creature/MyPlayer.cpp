// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/MyPlayer.h"
#include "MyRpg.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ActorComponents/InventoryComponent.h"
#include "ActorComponents/SkillComponent.h"
#include "ActorComponents/QuestComponent.h"


//temp
#include "Npc.h"


AMyPlayer::AMyPlayer()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f; // The camera follows at this distance behind the character
	//CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectRange"));
	DetectionBox->InitBoxExtent(FVector(500.f, 500.0f, 500.0f));
	DetectionBox->SetCollisionProfileName(TEXT("InteractRange"));
	DetectionBox->SetupAttachment(RootComponent);
	DetectionBox->SetGenerateOverlapEvents(true);
}


void AMyPlayer::InitSkill(int32 SkillPoint)
{
	SkillComponent->SetSkillPoint(SkillPoint);
}

void AMyPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());


	GInstance->SetGPlayer(this);
	PlayerController->Init();
	Inventory->SetOwnerPlayer(this);
	SkillComponent->Init(this);
	QuestComponent->Init(this);
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnDetectOverlapBegin);
	DetectionBox->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnDetectOverlapEnd);
	SendLoadInfoPacket();
}


void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveOnTick(DeltaTime);
}



void AMyPlayer::MoveOnTick(float DeltaTime)
{
	if (PlayerController == nullptr)
		return;

	// 움직일 일이 없는 경우에는 굳이 패킷을 보내지 않게 함
	if (GetObjectInfo()->state() != Protocol::STATE_IDLE &&
		GetObjectInfo()->state() != Protocol::STATE_MOVE &&
		GetObjectInfo()->state() != Protocol::STATE_RUN)
		return;
	
	// 이동 방향이 전환 되었는가 확인
	bool ForceSendPacket = IsDirectionSwitched();

	// 이동 여부에 따른 상태 설정
	SetMoveOrIdleState();

	// 회전 값 동기화를 위한 값 설정
	SetHorizontal(PlayerController->Horizontal);
	SetVertical(PlayerController->Vertical);

	// 패킷 전송 타이머 갱신
	MovePacketSendTimer -= DeltaTime;

	// 패킷을 전송할 타이밍이거나 이동 방향 전환이 감지되면 이동 패킷 전송
	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;
		SendMovePacket();
	}
}

void AMyPlayer::Attack(FRotator Rotation)
{
	if(bCanAttack == false)
		return;
	
	if(AttackSequence >= MaxAttackSequence)
		return;

	bCanAttack = false;
	// 1. 상태를 Attack으로 변경
	SetState(Protocol::EState::STATE_ATTACK);
	AttackSequence = AttackSequence + 1;

	// 2. 공격 패킷 보내기
	SendMovePacket(); // 동기화를 위한 이동 패킷 한 번 보내고
	SendAttackPacket(); // 공격 패킷
	
	Super::Attack(Rotation);
}

void AMyPlayer::UseSkill(int32 SkillId)
{
	Super::UseSkill(SkillId);
}

void AMyPlayer::OnDamaged(float Damage, uint64 AttackerId)
{
	Super::OnDamaged(Damage, AttackerId);

}

void AMyPlayer::Interact()
{
	if (InteractingNpc == nullptr)
	{
		return;
	}
	InteractingNpc->Interact();
}

void AMyPlayer::OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with Not MyPlayer %s"), *OtherActor->GetName());
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (Cast<ARpgPlayer>(OtherActor) != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with %s"), *OtherActor->GetName());
			Cast<ARpgPlayer>(OtherActor)->SetHpBarVisibility(true);
		}
	}
}

void AMyPlayer::OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with Not MyPlayer %s"), *OtherActor->GetName());
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (Cast<ARpgPlayer>(OtherActor) != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with %s"), *OtherActor->GetName());
			Cast<ARpgPlayer>(OtherActor)->SetHpBarVisibility(false);
		}
	}
}

#pragma region Send Packet Functions
void AMyPlayer::SendMovePacket()
{
	Protocol::C_MOVE MovePkt;

	// 현재 위치 정보
	{
		Protocol::ObjectInfo* Info = MovePkt.mutable_info();
		Protocol::PlayerMoveInfo* ControllInfo = MovePkt.mutable_playerinfo();
		Info->CopyFrom(*ObjectInfo);
		Info->set_state(GetState());
		ControllInfo->set_yaw(PlayerController->DesiredYaw);
		ControllInfo->set_dirx(PlayerController->DesiredMoveDirection.X);
		ControllInfo->set_diry(PlayerController->DesiredMoveDirection.Y);
		ControllInfo->set_dirz(PlayerController->DesiredMoveDirection.Z);
		ControllInfo->set_horizontal(PlayerController->Horizontal);
		ControllInfo->set_vertical(PlayerController->Vertical);
	}

	SEND_PACKET(MovePkt);
}
void AMyPlayer::SendAttackPacket()
{
	Protocol::C_ATTACK AttackPkt;
	{
		Protocol::ObjectInfo* Info = AttackPkt.mutable_info();
		Info->CopyFrom(*ObjectInfo);
		AttackPkt.set_attack_sequence(AttackSequence);
		AttackPkt.set_x(GetActorRotation().Roll);
		AttackPkt.set_y(GetActorRotation().Pitch);
		AttackPkt.set_z(GetActorRotation().Yaw);

	}
	SEND_PACKET(AttackPkt);
}

void AMyPlayer::SendLoadInfoPacket()
{
	// Temp : 서버에 캐릭터 정보 업데이트 해줘 패킷 보내기
	Protocol::C_LOAD_INFO Pkt;
	Pkt.set_object_id(ObjectInfo->object_id());

	SEND_PACKET(Pkt);
}

bool AMyPlayer::IsDirectionSwitched()
{
	if (PlayerController->LastDesiredInput != PlayerController->DesiredInput)
	{
		return true;
	}

	return false;
}

void AMyPlayer::SetMoveOrIdleState()
{
	if (PlayerController->DesiredInput == FVector2D::Zero())
	{
		SetState(Protocol::STATE_IDLE);
	}
	else
	{
		SetState(Protocol::STATE_RUN);
	}
}

void AMyPlayer::SendSkillPacket(int32 SkillId)
{

	SetState(Protocol::EState::STATE_ATTACK);
	SendMovePacket(); // 동기화를 위한 이동 패킷 한 번 보내고
	Protocol::C_USESKILL SkillPkt;

	Protocol::ObjectInfo* Info = SkillPkt.mutable_info();
	Info->CopyFrom(*ObjectInfo);
	SkillPkt.set_skillid(SkillId);
	SkillPkt.set_x(GetActorRotation().Roll);
	SkillPkt.set_y(GetActorRotation().Pitch);
	SkillPkt.set_z(GetActorRotation().Yaw);
	SEND_PACKET(SkillPkt);
}
#pragma endregion


