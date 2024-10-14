// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Protocol.pb.h"
#include "SlotDrag.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API USlotDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UItem> Item;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkill> Skill;
	

	// 나중에 슬롯 타입도 추가하자. -> 스킬 UI 만들고 나면 .
};
