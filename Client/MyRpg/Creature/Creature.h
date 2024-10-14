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
		모든 Creature가 상속받아 실행할 함수
		관련된 패킷을 받으면 실행할 함수들이다.
	*/
	virtual void MoveOnTick(float DeltaTime) PURE_VIRTUAL(ACreature::MoveOnTick, );
	virtual void Attack(FRotator Rotator) PURE_VIRTUAL(ACreature::Attack, );
	virtual void OnDamaged(float Damage, uint64 AttackerId);
	virtual void Die();
	virtual void OnStatChanged(const Protocol::StatInfo& Info);

	virtual void RefreshHpBar();
public:
	/* Get / Set 함수 모음 */
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
	/* 씬에서 보거나 건드릴 수 있는 컴포넌트 */
	UPROPERTY()
	TArray<TObjectPtr<class UCreatureAnimInstance>> AnimInsts;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> DamageTextComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> HpBar;

protected:
	/* 시각적으로 볼 수 없고 특정 기능만을 위한 컴포넌트 객체들 */
	UPROPERTY()
	TObjectPtr<class UStatComponent> StatComponent;

protected:
	// Temp
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> AudioComponent;
protected:
	/* 객체가 필요로 하는 정보 변수들
		
		ObjectInfo : 객체의 기본적인 정보가 담김
		DestPos : 조작이 없이 움직여야 하는 객체 (몬스터, 타플레이어 등)의 이동 목표 지점 설정을 위한 변수
		AttackSequence : 현재 진행 중인 공격 콤보
		MaxAttackSequence : 최대 콤보
		bCanAttack : 현재 공격가능 여부
	*/

	class Protocol::ObjectInfo* ObjectInfo; 
	class Protocol::ObjectInfo* DestPos;
	int32 AttackSequence;
	int32 MaxAttackSequence;
	bool bCanAttack;
};
