// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Creature.h"
#include "Protocol.pb.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RpgPlayer.generated.h"

UCLASS()
class MYRPG_API ARpgPlayer : public ACreature
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARpgPlayer();
	virtual ~ARpgPlayer();

	virtual void Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name) override;
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	/*
			��� Creature�� ��ӹ޾� ������ �Լ�
			���õ� ��Ŷ�� ������ ������ �Լ����̴�.
	*/
	virtual void MoveOnTick(float DeltaTime) override;
	virtual void Attack(FRotator Rotator) override;
	virtual void UseSkill(int32 SkillId);
	virtual void OnDamaged(float Damage, uint64 AttackerId) override;
public:
	bool IsMyPlayer();
	FString GetPlayerName() { return PlayerName; }
	float GetHorizontal() { return MoveInfo->horizontal(); }
	float GetVertical() { return MoveInfo->vertical(); }

	void SetHorizontal(float Value) { MoveInfo->set_horizontal(Value); }
	void SetVertical(float Value) { MoveInfo->set_vertical(Value); }

	void SetMoveInfo(const Protocol::PlayerMoveInfo& Info) { MoveInfo->CopyFrom(Info); }

public:
	void SetHpBarVisibility(bool Value);

private:
	void SynchronizePosition();
	void SetAnimClass(Protocol::ECharacterType Type);
	void SetWeapon(Protocol::ECharacterType Type);

public:
	// ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WizardWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WarriorWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RogueWeaponSocketL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RogueWeaponSocketR;
protected:

	UPROPERTY()
	FString PlayerName;

	/* �ٸ� �÷��̾ ���� ���۵Ǿ� �� ȭ�鿡���� �������� �ϴ� �÷��̾��� ���� ���� ���� */
	class Protocol::PlayerMoveInfo* MoveInfo;
};
