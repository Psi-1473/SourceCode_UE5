// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Creature.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class MYRPG_API AMonster : public ACreature
{
	GENERATED_BODY()

public:
	AMonster();

	virtual void Init(const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString Name) override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/*
			모든 Creature가 상속받아 실행할 함수
			관련된 패킷을 받으면 실행할 함수들이다.
	*/
	virtual void MoveOnTick(float DeltaTime) override;
	virtual void Attack(FRotator Rotator) override;
	virtual void OnDamaged(float Damage, uint64 AttackerId) override;

public:
	/* Get / Set 함수들 */
	virtual void SetState(Protocol::EState State) override;
	
	int32 GetTemplateId() { return TemplateId; }

private:
	void SetBaseStatInfo();
	void UpdateDead(float DeltaTime);
	void SetSpeed();
private:
	struct FMonsterStatData* BaseStat;
	int32 TemplateId;
	int32 Hp;
private:
	float DeadTick = 3.0f;
};
