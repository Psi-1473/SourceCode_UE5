// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/RpgPlayer.h"
#include "MyPlayer.generated.h"



UCLASS()
class MYRPG_API AMyPlayer : public ARpgPlayer
{
	GENERATED_BODY()

public:
	AMyPlayer();

	void InitSkill(int32 SkillPoint);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	/*
		모든 Creature가 상속받아 실행할 함수
		관련된 패킷을 받으면 실행할 함수들이다.
		*** MyPlayer에서는 애니메이션 관련 부분은 클라에서 먼저 실행한다.
	*/
	virtual void MoveOnTick(float DeltaTime) override;
	virtual void Attack(FRotator Rotation) override;
	virtual void UseSkill(int32 SkillId) override;
	virtual void OnDamaged(float Damage, uint64 AttackerId) override;
	void Interact();

	UFUNCTION()
	void OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


public:
	void SetInteractingNpc(TObjectPtr<class ANpc> NewNpc) { InteractingNpc = NewNpc; }

public:
	/* Inventory 관련 함수들 */
	TObjectPtr<class UInventoryComponent> GetInventory() { return Inventory; }

public:
	/* Skill Component 관련 함수들 */
	TObjectPtr<class USkillComponent> GetSkillComponent() { return SkillComponent; }
	TObjectPtr<class UQuestComponent> GetQuestComponent() { return QuestComponent; }
public:
	void SendSkillPacket(int32 SkillId);
private:
	/* 서버에 패킷을 전송하는 함수들 */
	void SendMovePacket();
	void SendAttackPacket();
	void SendLoadInfoPacket();
	
	bool IsDirectionSwitched();
	void SetMoveOrIdleState();

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY()
	TObjectPtr<class AMyPlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<class ANpc> InteractingNpc;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	UPROPERTY()
	TObjectPtr<class USkillComponent> SkillComponent;

	UPROPERTY()
	TObjectPtr<class UQuestComponent> QuestComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> DetectionBox;
	
protected:
	const float MOVE_PACKET_SEND_DELAY = 0.1f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;



};
