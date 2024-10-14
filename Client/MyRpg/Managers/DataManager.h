// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Protocol.pb.h"
#include "Data/DataEnum.h"
#include "Data/DataStruct.h"
#include "Engine/DataTable.h"

#include "DataManager.generated.h"

/**
 * 
 */
UCLASS()
class MYRPG_API UDataManager : public UObject
{
	GENERATED_BODY()

public:
	UDataManager();
	void Init(TObjectPtr<class UMyGameInstance> Instance);

public:
	FString GetUIPath(FString UIType);
	FString GetScenePath(ESceneType Type);
	FNpcData* GetNpcData(int NpcId);
	FWeaponData* GetWeaponData(int TemplateId);
	FArmorData* GetArmorData(int TemplateId);
	FConsumableData* GetConsumableData(int TemplateId);
	FPlayerStatData* GetWarriorStatData(int Level);
	FPlayerStatData* GetRogueStatData(int Level);
	FPlayerStatData* GetWizardStatData(int Level);

	TObjectPtr<class USkill> GetSkill(Protocol::ECharacterType Type, int Id);
	FSkillInfoData* GetWarriorSkillData(int Level);
	FSkillInfoData* GetRogueSkillData(int Level);
	FSkillInfoData* GetWizardSkillData(int Level);
	FMonsterStatData* GetMonsterStatData(int Id);
	struct FRichImageRow* GetInteractListImage(int Id);
	struct FRichImageRow* GetGeneralImage(FString Key);
	struct FRichImageRow* GetWeaponImage(int Id);
	struct FRichImageRow* GetArmorImage(int Id);
	struct FRichImageRow* GetConsumableImage(int Id);
	
	struct FRichImageRow* GetSkillImage(Protocol::ECharacterType Type, int Id);
	struct FRichImageRow* GetWarriorSkillImage(int Id);
	struct FRichImageRow* GetRogueSkillImage(int Id);
	struct FRichImageRow* GetWizardSkillImage(int Id);
	struct FRichImageRow* GetClassIconImage(Protocol::ECharacterType Type);
	struct FRichImageRow* GetClassPortraitImage(Protocol::ECharacterType Type);

	struct FQuestData* GetQuestData(int32 QuestId);
	TSharedPtr<struct FQuestTargets> GetQuestTargetData(int32 QuestId);
	TSharedPtr<struct FQuestDialogue> GetQuestDialogue(int32 QuestId, int32 NpcId, int32 Type);
	TSharedPtr<struct FQuestReward> GetQuestRewards(int32 QuestId);
public:
	void SetSkillInfo();

private:
	void InitQuestTargetInfo();
	void InitQuestDialogue();
	void InitQuestRewardInfo();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> UIPathTable;

	UPROPERTY()
	TObjectPtr<UDataTable> ScenePathTable;

	UPROPERTY()
	TObjectPtr<UDataTable> NpcData;

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponData;

	UPROPERTY()
	TObjectPtr<UDataTable> ArmorData;

	UPROPERTY()
	TObjectPtr<UDataTable> ConsumableData;

	UPROPERTY()
	TObjectPtr<UDataTable> WarriorStatData;

	UPROPERTY()
	TObjectPtr<UDataTable> RogueStatData;

	UPROPERTY()
	TObjectPtr<UDataTable> WizardStatData;

	UPROPERTY()
	TObjectPtr<UDataTable> MonsterStatData;

	UPROPERTY()
	TObjectPtr<UDataTable> WarriorSkillData;

	UPROPERTY()
	TObjectPtr<UDataTable> RogueSkillData;

	UPROPERTY()
	TObjectPtr<UDataTable> WizardSkillData;
private:
	/* Image Data */
	UPROPERTY()
	TObjectPtr<UDataTable> InteractListImage;

	UPROPERTY()
	TObjectPtr<UDataTable> GeneralImage;

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponImage;

	UPROPERTY()
	TObjectPtr<UDataTable> ArmorImage;

	UPROPERTY()
	TObjectPtr<UDataTable> ConsumableImage;

	UPROPERTY()
	TObjectPtr<UDataTable> WarriorSkillImage;

	UPROPERTY()
	TObjectPtr<UDataTable> RogueSkillImage;

	UPROPERTY()
	TObjectPtr<UDataTable> WizardSkillImage;

	UPROPERTY()
	TObjectPtr<UDataTable> ClassIconImage;

	UPROPERTY()
	TObjectPtr<UDataTable> ClassPortraitImage;

	UPROPERTY()
	TWeakObjectPtr<class UMyGameInstance> GInstance;

private:
	/* class Data */
	UPROPERTY()
	TArray<TObjectPtr<class USkill>> WarriorSkills;

	UPROPERTY()
	TArray<TObjectPtr<class USkill>> RogueSkills;

	UPROPERTY()
	TArray<TObjectPtr<class USkill>> WizardSkills;

private:
	/* Quest Data */ 
	UPROPERTY()
	TObjectPtr<UDataTable> QuestData;

	TMap<int32, TSharedPtr<FQuestTargets>> QuestTargetData;
	TMap<FString, TSharedPtr<FQuestDialogue>> StartQuestDialogue;
	TMap<FString, TSharedPtr<FQuestDialogue>> CompleteQuestDialogue;
	TMap<FString, TSharedPtr<FQuestDialogue>> TargetQuestDialogue;
	TMap<int32, TSharedPtr<FQuestReward>> QuestRewards;

	
};
