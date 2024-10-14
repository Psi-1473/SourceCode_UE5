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
		��� Creature�� ��ӹ޾� ������ �Լ�
		���õ� ��Ŷ�� ������ ������ �Լ����̴�.
		*** MyPlayer������ �ִϸ��̼� ���� �κ��� Ŭ�󿡼� ���� �����Ѵ�.
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
	/* Inventory ���� �Լ��� */
	TObjectPtr<class UInventoryComponent> GetInventory() { return Inventory; }

public:
	/* Skill Component ���� �Լ��� */
	TObjectPtr<class USkillComponent> GetSkillComponent() { return SkillComponent; }
	TObjectPtr<class UQuestComponent> GetQuestComponent() { return QuestComponent; }
public:
	void SendSkillPacket(int32 SkillId);
private:
	/* ������ ��Ŷ�� �����ϴ� �Լ��� */
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
