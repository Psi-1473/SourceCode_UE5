// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Engine/DataTable.h"
#include "DataStruct.generated.h"
/**
	Data 형식을 정의한 구조체 모음
 */

USTRUCT(BlueprintType)
struct FUiPath : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Path;
 };

USTRUCT(BlueprintType)
struct FNpcData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int NpcId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString NpcName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int NpcType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString DefaultGreeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Items;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int TemplateId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int SellPrice;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int UsableLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Class;
};

USTRUCT(BlueprintType)
struct FArmorData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int UsableLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Class;
};

USTRUCT(BlueprintType)
struct FBaseStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Lvl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Defense;
};

USTRUCT(BlueprintType)
struct FPlayerStatData : public FBaseStatData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxExp;
};

USTRUCT(BlueprintType)
struct FMonsterStatData : public FBaseStatData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int monsterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString mobName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Exp;

};

USTRUCT(BlueprintType)
struct FSkillInfoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SkillDesc;
};

USTRUCT(BlueprintType)
struct FConsumableData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int HealType;
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 QuestId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString QuestDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 QuestGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 StartNpc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CompleteNpc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Gold;
};

USTRUCT(BlueprintType)
struct FQuestTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 QuestId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 TargetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString TargetDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 TargetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 TargetNumber;
};

USTRUCT(BlueprintType)
struct FQuestTargets
{
	GENERATED_BODY()

public:
	TArray<TSharedPtr<FQuestTargetData>> Targets;
};

USTRUCT(BlueprintType)
struct FQuestDialogueInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 QuestId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NpcId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Type;
};

USTRUCT(BlueprintType)
struct FQuestDialogue
{
	GENERATED_BODY()

public:
	TMap<int32, TSharedPtr<FQuestDialogueInfo>> Dialogue;
};

USTRUCT(BlueprintType)
struct FQuestRewardInfo
{
	GENERATED_BODY()

public:
	Protocol::EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemCount;
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

public:
	TArray<TSharedPtr<FQuestRewardInfo>> Rewards;
};
