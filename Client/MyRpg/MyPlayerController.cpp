// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Popup/InventoryWidget.h"
#include "Popup/EquipmentWidget.h"
#include "Popup/SkillWidget.h"
#include "Popup/PlayerQuestWidget.h"
#include "ActorComponents/InventoryComponent.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	//MyPlayer = Cast<AMyPlayer>(GetPawn());
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::Move);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Attack);

		// UI 혹은 기타 기능 관련 키 세팅
		EnhancedInputComponent->BindAction(Click_I_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_I);
		EnhancedInputComponent->BindAction(Click_F_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_F);
		EnhancedInputComponent->BindAction(Click_P_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_P);
		EnhancedInputComponent->BindAction(Click_Q_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_Q);
		EnhancedInputComponent->BindAction(Click_E_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_E);
		EnhancedInputComponent->BindAction(Click_R_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_R);
		EnhancedInputComponent->BindAction(Click_C_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_C);
		EnhancedInputComponent->BindAction(Click_J_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_J);
		EnhancedInputComponent->BindAction(Click_V_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_V);
		EnhancedInputComponent->BindAction(Click_T_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_T);
		EnhancedInputComponent->BindAction(Click_K_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_K);
		EnhancedInputComponent->BindAction(Click_1_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_1);
		EnhancedInputComponent->BindAction(Click_2_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_2);
		EnhancedInputComponent->BindAction(Click_3_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_3);
		EnhancedInputComponent->BindAction(Click_4_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_4);
		EnhancedInputComponent->BindAction(Click_5_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_5);
		EnhancedInputComponent->BindAction(Click_6_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_6);
		EnhancedInputComponent->BindAction(Click_7_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_7);
		EnhancedInputComponent->BindAction(Click_8_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_8);
		EnhancedInputComponent->BindAction(Click_9_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_9);
		EnhancedInputComponent->BindAction(Click_0_Acition, ETriggerEvent::Triggered, this, &AMyPlayerController::Click_0);
	}
}

void AMyPlayerController::Init()
{
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	OnClickedQuickSlot.BindUObject(GInstance, &UMyGameInstance::ClickQuickSlot);
}

void AMyPlayerController::StopMove()
{
	DesiredInput = FVector2D::Zero();
}


void AMyPlayerController::Look(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	// add yaw and pitch input to controller

	UE_LOG(LogTemp, Warning, TEXT("Rotation : %f, %f"), LookAxisVector.X / 10, LookAxisVector.Y / 10);
	MyPlayer->AddControllerYawInput(LookAxisVector.X / 3);
	MyPlayer->AddControllerPitchInput((LookAxisVector.Y / 3) * -1);
}


void AMyPlayerController::Move(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	if(MyPlayer->GetObjectInfo()->state() != Protocol::STATE_IDLE &&
		MyPlayer->GetObjectInfo()->state() != Protocol::STATE_MOVE &&
		MyPlayer->GetObjectInfo()->state() != Protocol::STATE_RUN)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	
	Horizontal = MovementVector.X;
	Vertical = MovementVector.Y;
	
	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// add movement 
	MyPlayer->AddMovementInput(ForwardDirection, MovementVector.Y);
	MyPlayer->AddMovementInput(RightDirection, MovementVector.X);
	
	// Cache
	{
		DesiredInput = MovementVector;
	
		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * MovementVector.Y;
		DesiredMoveDirection += RightDirection * MovementVector.X;
		DesiredMoveDirection.Normalize();
		
		const FVector Location = MyPlayer->GetActorLocation();
		//
		FRotator Rotator = MyPlayer->GetActorRotation(); //UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection) : 
		DesiredYaw = Rotator.Yaw;
	}
}

void AMyPlayerController::Attack(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	if (MyPlayer->GetObjectInfo()->state() != Protocol::STATE_IDLE &&
		MyPlayer->GetObjectInfo()->state() != Protocol::STATE_MOVE &&
		MyPlayer->GetObjectInfo()->state() != Protocol::STATE_RUN &&
		MyPlayer->GetObjectInfo()->state() != Protocol::STATE_ATTACK)
		return;
	DesiredInput = FVector2D::Zero();

	if(Value.Get<bool>() == true)
	{
		MyPlayer->Attack(MyPlayer->GetActorRotation());
	}
}

void AMyPlayerController::Click_I(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GetGameInstance());

	TObjectPtr<UInventoryWidget> Widget = Cast<UInventoryWidget>(GInstance->GetUIMgr()->PopupUI("Inventory"));
	if(Widget != nullptr)
	{
		Widget->Init(MyPlayer->GetInventory());
	}
}

void AMyPlayerController::Click_P(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GetGameInstance());

	TObjectPtr<UEquipmentWidget> Widget = Cast<UEquipmentWidget>(GInstance->GetUIMgr()->PopupUI("Equipment"));
	if (Widget != nullptr)
	{
		Widget->Init(GInstance);
	}
}

void AMyPlayerController::Click_F(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	MyPlayer->Interact();
}

void AMyPlayerController::Click_Q(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_Q);
}

void AMyPlayerController::Click_E(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_E);
}

void AMyPlayerController::Click_R(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_R);
}

void AMyPlayerController::Click_C(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_C);
}

void AMyPlayerController::Click_J(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GetGameInstance());

	TObjectPtr<UPlayerQuestWidget> Widget = Cast<UPlayerQuestWidget>(GInstance->GetUIMgr()->PopupUI("PlayerQuest"));
	if (Widget != nullptr)
	{
		Widget->Init(GInstance);
	}
}

void AMyPlayerController::Click_V(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_V);
}

void AMyPlayerController::Click_T(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_T);
}

void AMyPlayerController::Click_K(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GetGameInstance());

	TObjectPtr<USkillWidget> Widget = Cast<USkillWidget>(GInstance->GetUIMgr()->PopupUI("Skill"));
	if (Widget != nullptr)
	{
		Widget->Init(GInstance);
	}
}

void AMyPlayerController::Click_1(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_1);
}

void AMyPlayerController::Click_2(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_2);
}

void AMyPlayerController::Click_3(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_3);
}

void AMyPlayerController::Click_4(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_4);
}

void AMyPlayerController::Click_5(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_5);
}

void AMyPlayerController::Click_6(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_6);
}

void AMyPlayerController::Click_7(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_7);
}

void AMyPlayerController::Click_8(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_8);
}

void AMyPlayerController::Click_9(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_9);
}

void AMyPlayerController::Click_0(const FInputActionValue& Value)
{
	if (MyPlayer == nullptr)
	{
		return;
	}
	OnClickedQuickSlot.Execute(Protocol::QUICK_0);
}

