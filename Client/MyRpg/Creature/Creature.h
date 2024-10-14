// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "GameFramework/Character.h"
#include "ActorComponents/StatComponent.h"
#include "Creature.generated.h"

UCLASS(ABSTRACT)
class MYRPG_API ACreature : public ACharacter
{
	GENERATED_BODY()

public:
	ACreature();

	virtual void Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/*
		��� Creature�� ��ӹ޾� ������ �Լ�
		���õ� ��Ŷ�� ������ ������ �Լ����̴�.
	*/
	virtual void MoveOnTick(float DeltaTime) PURE_VIRTUAL(ACreature::MoveOnTick, );
	virtual void Attack(FRotator Rotator) PURE_VIRTUAL(ACreature::Attack, );
	virtual void OnDamaged(float Damage, uint64 AttackerId);
	virtual void Die();
	virtual void OnStatChanged(const Protocol::StatInfo& Info);

	virtual void RefreshHpBar();
public:
	/* Get / Set �Լ� ���� */
	TObjectPtr<class UStatComponent> GetStatComponent() { return StatComponent; }
	TObjectPtr<class UAudioComponent> GetAudioComponent() { return AudioComponent; }


	Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }
	Protocol::EState GetState() { return ObjectInfo->state(); }
	

	virtual void SetState(Protocol::EState State);
	void SetObjectInfo(const Protocol::ObjectInfo& Info);
	void SetDestInfo(const Protocol::ObjectInfo& Info);
	void SetAttackSequence(int32 Value) { AttackSequence = Value; }
	void SetCanAttack(bool Value) { bCanAttack = Value; }
	


protected:
	/* ������ ���ų� �ǵ帱 �� �ִ� ������Ʈ */
	UPROPERTY()
	TArray<TObjectPtr<class UCreatureAnimInstance>> AnimInsts;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> DamageTextComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> HpBar;

protected:
	/* �ð������� �� �� ���� Ư�� ��ɸ��� ���� ������Ʈ ��ü�� */
	UPROPERTY()
	TObjectPtr<class UStatComponent> StatComponent;

protected:
	// Temp
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> AudioComponent;
protected:
	/* ��ü�� �ʿ�� �ϴ� ���� ������
		
		ObjectInfo : ��ü�� �⺻���� ������ ���
		DestPos : ������ ���� �������� �ϴ� ��ü (����, Ÿ�÷��̾� ��)�� �̵� ��ǥ ���� ������ ���� ����
		AttackSequence : ���� ���� ���� ���� �޺�
		MaxAttackSequence : �ִ� �޺�
		bCanAttack : ���� ���ݰ��� ����
	*/

	class Protocol::ObjectInfo* ObjectInfo; 
	class Protocol::ObjectInfo* DestPos;
	int32 AttackSequence;
	int32 MaxAttackSequence;
	bool bCanAttack;
};
