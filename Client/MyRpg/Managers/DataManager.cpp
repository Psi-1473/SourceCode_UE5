// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DataManager.h"
#include "MyGameInstance.h"
#include "Skill/Skill.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "Json.h"
#include "JsonUtilities.h"


UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> UiPath(TEXT("Script/Engine.DataTable'/Game/DataTable/UIPath/DT_UiPath.DT_UiPath'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ScenePath(TEXT("/Script/Engine.DataTable'/Game/DataTable/UIPath/DT_ScenePath.DT_ScenePath'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Weapon(TEXT("/Script/Engine.DataTable'/Game/DataTable/Item/WeaponData.WeaponData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Armor(TEXT("/Script/Engine.DataTable'/Game/DataTable/Item/ArmorData.ArmorData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Consumable(TEXT("/Script/Engine.DataTable'/Game/DataTable/Item/ConsumableData.ConsumableData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Npc(TEXT("/Script/Engine.DataTable'/Game/DataTable/Npc/NpcData.NpcData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> InterList(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/InteractListImage.InteractListImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/WeaponImage.WeaponImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ArmorImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/ArmorImage.ArmorImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> ConsumableImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/ConsumableImage.ConsumableImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WarriorSkillImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/WarriorSkillImage.WarriorSkillImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> RogueSkillImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/RogueSkillImage.RogueSkillImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WizardSkillImg(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/WizardSkillImage.WizardSkillImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WarriorStat(TEXT("/Script/Engine.DataTable'/Game/DataTable/PlayerStat/WarriorStatData.WarriorStatData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> RogueStat(TEXT("/Script/Engine.DataTable'/Game/DataTable/PlayerStat/RogueStatData.RogueStatData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WizardStat(TEXT("/Script/Engine.DataTable'/Game/DataTable/PlayerStat/WizardStatData.WizardStatData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MobStat(TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterStat/MonsterStatData.MonsterStatData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WarriorSkill(TEXT("/Script/Engine.DataTable'/Game/DataTable/Skill/WarriorSkillInfo.WarriorSkillInfo'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> RogueSkill(TEXT("/Script/Engine.DataTable'/Game/DataTable/Skill/RogueSkillInfo.RogueSkillInfo'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WizardSkill(TEXT("/Script/Engine.DataTable'/Game/DataTable/Skill/WizardSkillInfo.WizardSkillInfo'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Icon(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/ClassIconImage.ClassIconImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Portrait(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/ClassPortraitImage.ClassPortraitImage'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Quest(TEXT("/Script/Engine.DataTable'/Game/DataTable/Quest/QuestInfo.QuestInfo'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> GImage(TEXT("/Script/Engine.DataTable'/Game/DataTable/Image/GeneralImage.GeneralImage'"));

	if (UiPath.Succeeded())
	{
		UIPathTable = UiPath.Object;
	}
	if (ScenePath.Succeeded())
	{
		ScenePathTable = ScenePath.Object;
	}
	if (Weapon.Succeeded())
	{
		WeaponData = Weapon.Object;
	}
	if (Armor.Succeeded())
	{
		ArmorData = Armor.Object;
	}
	if (Consumable.Succeeded())
	{
		ConsumableData = Consumable.Object;
	}
	if (Npc.Succeeded())
	{
		NpcData = Npc.Object;
	}
	if (InterList.Succeeded())
	{
		InteractListImage = InterList.Object;
	}
	if (WeaponImg.Succeeded())
	{
		WeaponImage = WeaponImg.Object;
	}
	if (ArmorImg.Succeeded())
	{
		ArmorImage = ArmorImg.Object;
	}
	if (ConsumableImg.Succeeded())
	{
		ConsumableImage = ConsumableImg.Object;
	}
	if (WarriorStat.Succeeded())
	{
		WarriorStatData = WarriorStat.Object;
	}
	if (RogueStat.Succeeded())
	{
		RogueStatData = RogueStat.Object;
	}
	if (WizardStat.Succeeded())
	{
		WizardStatData = WizardStat.Object;
	}
	if (MobStat.Succeeded())
	{
		MonsterStatData = MobStat.Object;
	}
	if (WarriorSkillImg.Succeeded())
	{
		WarriorSkillImage = WarriorSkillImg.Object;
	}
	if (RogueSkillImg.Succeeded())
	{
		RogueSkillImage = RogueSkillImg.Object;
	}
	if (WizardSkillImg.Succeeded())
	{
		WizardSkillImage = WizardSkillImg.Object;
	}
	if (WarriorSkill.Succeeded())
	{
		WarriorSkillData = WarriorSkill.Object;
	}
	if (RogueSkill.Succeeded())
	{
		RogueSkillData = RogueSkill.Object;
	}
	if (WizardSkill.Succeeded())
	{
		WizardSkillData = WizardSkill.Object;
	}
	if (Icon.Succeeded())
	{
		ClassIconImage = Icon.Object;
	}
	if (Portrait.Succeeded())
	{
		ClassPortraitImage = Portrait.Object;
	}
	if (Quest.Succeeded())
	{
		QuestData = Quest.Object;
	}
	if (GImage.Succeeded())
	{
		GeneralImage = GImage.Object;
	}
}

void UDataManager::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
	InitQuestTargetInfo();
	InitQuestDialogue();
	InitQuestRewardInfo();
}

FString UDataManager::GetUIPath(FString UIType)
{
	FUiPath* Row = nullptr;

	Row = UIPathTable->FindRow<FUiPath>(FName(*UIType), TEXT(""));

	return Row->Path;
}

FString UDataManager::GetScenePath(ESceneType Type)
{
	FUiPath* Row = nullptr;
	int32 Index = (int32)Type;
	Row = ScenePathTable->FindRow<FUiPath>(FName(*FString::FromInt(Index)), TEXT(""));

	return Row->Path;
}

FNpcData* UDataManager::GetNpcData(int NpcId)
{
	FNpcData* Data = NpcData->FindRow<FNpcData>(FName(*FString::FromInt(NpcId)), TEXT(""));
	return Data;
}

FWeaponData* UDataManager::GetWeaponData(int TemplateId)
{
	FWeaponData* Data = WeaponData->FindRow<FWeaponData>(FName(*FString::FromInt(TemplateId)), TEXT(""));
	return Data;
}

FArmorData* UDataManager::GetArmorData(int TemplateId)
{
	FArmorData* Data = ArmorData->FindRow<FArmorData>(FName(*FString::FromInt(TemplateId)), TEXT(""));
	return Data;
}

FConsumableData* UDataManager::GetConsumableData(int TemplateId)
{
	FConsumableData* Data = ConsumableData->FindRow<FConsumableData>(FName(*FString::FromInt(TemplateId)), TEXT(""));
	return Data;
}

FPlayerStatData* UDataManager::GetWarriorStatData(int Level)
{
	FPlayerStatData* Data = WarriorStatData->FindRow<FPlayerStatData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

FPlayerStatData* UDataManager::GetRogueStatData(int Level)
{
	FPlayerStatData* Data = RogueStatData->FindRow<FPlayerStatData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

FPlayerStatData* UDataManager::GetWizardStatData(int Level)
{
	FPlayerStatData* Data = WizardStatData->FindRow<FPlayerStatData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

TObjectPtr<class USkill> UDataManager::GetSkill(Protocol::ECharacterType Type, int Id)
{
	switch (Type)
	{
	case Protocol::WARRIOR:
		return WarriorSkills[Id];
		break;
	case Protocol::ROGUE:
		return RogueSkills[Id];
		break;
	case Protocol::WIZARD:
		return WizardSkills[Id];
		break;
	default:
		return nullptr;
		break;
	}
}

FSkillInfoData* UDataManager::GetWarriorSkillData(int Level)
{
	FSkillInfoData* Data = WarriorSkillData->FindRow<FSkillInfoData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

FSkillInfoData* UDataManager::GetRogueSkillData(int Level)
{
	FSkillInfoData* Data = RogueSkillData->FindRow<FSkillInfoData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

FSkillInfoData* UDataManager::GetWizardSkillData(int Level)
{
	FSkillInfoData* Data = WizardSkillData->FindRow<FSkillInfoData>(FName(*FString::FromInt(Level)), TEXT(""));
	return Data;
}

FMonsterStatData* UDataManager::GetMonsterStatData(int Id)
{
	FMonsterStatData* Data = MonsterStatData->FindRow<FMonsterStatData>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetInteractListImage(int Id)
{
	FRichImageRow* Data = InteractListImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetGeneralImage(FString Key)
{
	FRichImageRow* Data = GeneralImage->FindRow<FRichImageRow>(FName(*Key), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetWeaponImage(int Id)
{
	FRichImageRow* Data = WeaponImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetArmorImage(int Id)
{
	FRichImageRow* Data = ArmorImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetConsumableImage(int Id)
{
	FRichImageRow* Data = ConsumableImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetSkillImage(Protocol::ECharacterType Type, int Id)
{
	switch (Type)
	{
		case Protocol::WARRIOR:
			return GetWarriorSkillImage(Id);
			break;
		case Protocol::ROGUE:
			return GetRogueSkillImage(Id);
			break;
		case Protocol::WIZARD:
			return GetWizardSkillImage(Id);
			break;
		default:
			return nullptr;
			break;
	}
}

FRichImageRow* UDataManager::GetWarriorSkillImage(int Id)
{
	FRichImageRow* Data = WarriorSkillImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetRogueSkillImage(int Id)
{
	FRichImageRow* Data = RogueSkillImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetWizardSkillImage(int Id)
{
	FRichImageRow* Data = WizardSkillImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Id)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetClassIconImage(Protocol::ECharacterType Type)
{
	FRichImageRow* Data = ClassIconImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Type)), TEXT(""));
	return Data;
}

FRichImageRow* UDataManager::GetClassPortraitImage(Protocol::ECharacterType Type)
{
	FRichImageRow* Data = ClassPortraitImage->FindRow<FRichImageRow>(FName(*FString::FromInt(Type)), TEXT(""));
	return Data;
}

FQuestData* UDataManager::GetQuestData(int32 QuestId)
{
	FQuestData* Data = QuestData->FindRow<FQuestData>(FName(*FString::FromInt(QuestId)), TEXT(""));
	return Data;
}

TSharedPtr<FQuestTargets> UDataManager::GetQuestTargetData(int32 QuestId)
{
	if(QuestTargetData.Contains(QuestId) == false)
	{
		return nullptr;
	}
	else
	{
		return QuestTargetData[QuestId];
	}
}

TSharedPtr<FQuestDialogue> UDataManager::GetQuestDialogue(int32 QuestId, int32 NpcId, int32 Type)
{
	FString Key = FString::Printf(TEXT("%d_%d"), NpcId, QuestId);
	if (Type == 0)
	{
		return StartQuestDialogue[Key];
	}
	if (Type == 1)
	{
		return CompleteQuestDialogue[Key];
	}
	if (Type == 2)
	{
		return TargetQuestDialogue[Key];
	}
	else
	{
		return nullptr;
	}
}

TSharedPtr<struct FQuestReward> UDataManager::GetQuestRewards(int32 QuestId)
{
	if (QuestRewards.Contains(QuestId))
	{
		return QuestRewards[QuestId];
	}

	return nullptr;
}

void UDataManager::SetSkillInfo()
{
	for (int i = 0; i < WarriorSkillData->GetRowMap().Num(); i++)
	{
		TObjectPtr<USkill> Skill = NewObject<USkill>();
		Skill->Init(GetWarriorSkillData(i), GetWarriorSkillImage(i));
		WarriorSkills.Add(Skill);
	}

	for (int i = 0; i < RogueSkillData->GetRowMap().Num(); i++)
	{
		TObjectPtr<USkill> Skill = NewObject<USkill>();
		Skill->Init(GetRogueSkillData(i), GetRogueSkillImage(i));
		RogueSkills.Add(Skill);
	}

	for (int i = 0; i < WarriorSkillData->GetRowMap().Num(); i++)
	{
		TObjectPtr<USkill> Skill = NewObject<USkill>();
		Skill->Init(GetWizardSkillData(i), GetWizardSkillImage(i));
		WizardSkills.Add(Skill);
	}
}

void UDataManager::InitQuestTargetInfo()
{
	// JSON 파일 경로 설정
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Json/QuestTargetInfo.json");

	// 파일 내용을 읽기 위한 변수 선언
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file."));
		return;
	}

	// JSON 파싱
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	UE_LOG(LogTemp, Error, TEXT("Start parsing JSON."));
	if (FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		for (const TSharedPtr<FJsonValue>& Value : JsonArray)
		{
			TSharedPtr<FJsonObject> JsonObject = Value->AsObject();

			// 데이터 읽기
			int32 QuestId = JsonObject->GetIntegerField(TEXT("questId"));
			int32 TargetIndex = JsonObject->GetIntegerField(TEXT("targetIndex"));
			FString TargetDesc = JsonObject->GetStringField(TEXT("targetDesc"));
			int32 TargetType = JsonObject->GetIntegerField(TEXT("targetType"));
			int32 TargetId = JsonObject->GetIntegerField(TEXT("targetId"));
			int32 TargetNumber = JsonObject->GetIntegerField(TEXT("targetNumber"));

			
			TSharedPtr<FQuestTargetData> Data = MakeShared<FQuestTargetData>();
			Data->QuestId = QuestId;
			Data->TargetIndex = TargetIndex;
			Data->TargetDesc = TargetDesc;
			Data->TargetType = TargetType;
			Data->TargetId = TargetId;
			Data->TargetNumber = TargetNumber;
			UE_LOG(LogTemp, Error, TEXT("Quest Target Info Load %d, %d, %d, %d, %d, %s"), QuestId, TargetIndex, TargetType, TargetId, TargetNumber, *TargetDesc);
			if (QuestTargetData.Contains(QuestId))
			{
				QuestTargetData[QuestId]->Targets.Add(Data);
			}
			else
			{
				TSharedPtr<FQuestTargets> Target = MakeShared<FQuestTargets>();
				Target->Targets.Add(Data);
				QuestTargetData.Add(QuestId, Target);
			}
		}

		UE_LOG(LogTemp, Error, TEXT("Succeed to parse JSON."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
	}
}

void UDataManager::InitQuestDialogue()
{
	// JSON 파일 경로 설정
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Json/NpcDialogueData.json");

	// 파일 내용을 읽기 위한 변수 선언
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file."));
		return;
	}

	// JSON 파싱
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	UE_LOG(LogTemp, Error, TEXT("Start parsing JSON."));
	if (FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		for (const TSharedPtr<FJsonValue>& Value : JsonArray)
		{
			TSharedPtr<FJsonObject> JsonObject = Value->AsObject();

			// 데이터 읽기
			int32 QuestId = JsonObject->GetIntegerField(TEXT("questId"));
			int32 NpcId = JsonObject->GetIntegerField(TEXT("npcId"));
			int32 Index = JsonObject->GetIntegerField(TEXT("index"));
			FString Dialogue = JsonObject->GetStringField(TEXT("dialogue"));
			int32 Type = JsonObject->GetIntegerField(TEXT("type"));


			TSharedPtr<FQuestDialogueInfo> Data = MakeShared<FQuestDialogueInfo>();
			Data->QuestId = QuestId;
			Data->NpcId = NpcId;
			Data->Index = Index;
			Data->Dialogue = Dialogue;
			Data->Type = Type;

			FString Key = FString::Printf(TEXT("%d_%d"), NpcId, QuestId);
		

			if (Type == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("Start Quest Target Info Load %d, %d, %d, %d, %s"), QuestId, NpcId, Index, Type, *Dialogue);
				if (StartQuestDialogue.Contains(Key))
				{
					StartQuestDialogue[Key]->Dialogue.Add(Index, Data);
				}
				else
				{
					TSharedPtr<FQuestDialogue> NpcDialogue = MakeShared<FQuestDialogue>();
					NpcDialogue->Dialogue.Add(Index, Data);
					StartQuestDialogue.Add(Key, NpcDialogue);
				}
			}
			else if (Type == 1)
			{
				UE_LOG(LogTemp, Error, TEXT("Complete Quest Target Info Load %d, %d, %d, %d, %s"), QuestId, NpcId, Index, Type, *Dialogue);
				if (CompleteQuestDialogue.Contains(Key))
				{
					CompleteQuestDialogue[Key]->Dialogue.Add(Index, Data);
				}
				else
				{
					TSharedPtr<FQuestDialogue> NpcDialogue = MakeShared<FQuestDialogue>();
					NpcDialogue->Dialogue.Add(Index, Data);
					CompleteQuestDialogue.Add(Key, NpcDialogue);
				}
			}
			else if (Type == 2)
			{
				UE_LOG(LogTemp, Error, TEXT("Target Quest Target Info Load %d, %d, %d, %d, %s"), QuestId, NpcId, Index, Type, *Dialogue);
				if (TargetQuestDialogue.Contains(Key))
				{
					TargetQuestDialogue[Key]->Dialogue.Add(Index, Data);
				}
				else
				{
					TSharedPtr<FQuestDialogue> NpcDialogue = MakeShared<FQuestDialogue>();
					NpcDialogue->Dialogue.Add(Index, Data);
					TargetQuestDialogue.Add(Key, NpcDialogue);
				}
			}
			
		}

		UE_LOG(LogTemp, Error, TEXT("Succeed to parse JSON."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
	}
}

void UDataManager::InitQuestRewardInfo()
{
	// JSON 파일 경로 설정
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Json/QuestRewards.json");

	// 파일 내용을 읽기 위한 변수 선언
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file."));
		return;
	}

	// JSON 파싱
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	UE_LOG(LogTemp, Error, TEXT("Start parsing JSON."));
	if (FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		for (const TSharedPtr<FJsonValue>& Value : JsonArray)
		{
			TSharedPtr<FJsonObject> JsonObject = Value->AsObject();

			// 데이터 읽기
			int32 QuestId = JsonObject->GetIntegerField(TEXT("questId"));
			Protocol::EItemType ItemType = static_cast<Protocol::EItemType>(JsonObject->GetIntegerField(TEXT("itemType")));
			int32 ItemId = JsonObject->GetIntegerField(TEXT("itemId"));
			int32 ItemCount = JsonObject->GetIntegerField(TEXT("itemCount"));


			TSharedPtr<FQuestRewardInfo> Data = MakeShared<FQuestRewardInfo>();
			Data->ItemId = ItemId;
			Data->ItemCount = ItemCount;
			Data->ItemType = ItemType;
	
			UE_LOG(LogTemp, Error, TEXT("Quest Reward Info Load %d, %d, %d"), QuestId, ItemId, ItemCount);
			if (QuestRewards.Contains(QuestId))
			{
				QuestRewards[QuestId]->Rewards.Add(Data);
			}
			else
			{
				TSharedPtr<FQuestReward> RewardList = MakeShared<FQuestReward>();
				RewardList->Rewards.Add(Data);
				QuestRewards.Add(QuestId, RewardList);
			}
		}

		UE_LOG(LogTemp, Error, TEXT("Succeed to parse JSON."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
	}
}
