// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/StatComponent.h"
#include "MyPlayer.h"
#include "Monster.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "Data/DataStruct.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	StatInfo = new Protocol::StatInfo();
}

UStatComponent::~UStatComponent()
{
	delete StatInfo;
}

void UStatComponent::Init(TObjectPtr<ACreature> Creature)
{
	OwnerCreature = Creature;
}


void UStatComponent::Refresh()
{
	if(StatChanged.IsBound())
	{
		StatChanged.Execute(Cast<UMyGameInstance>(OwnerCreature->GetWorld()->GetGameInstance()));
	}
}

float UStatComponent::GetHpRatio()
{
	FBaseStatData* StatData = GetStatData();
	if (StatData == nullptr)
	{
		return -1.0f;
	}

	return (float)StatInfo->hp() / StatData->MaxHp;
}

float UStatComponent::GetMpRatio()
{
	FBaseStatData* StatData = GetStatData();
	if (StatData == nullptr)
	{
		return -1.0f;
	}

	return (float)StatInfo->mp() / StatData->MaxMp;
}

float UStatComponent::GetExpRatio()
{
	FBaseStatData* StatData = GetStatData();
	if (StatData == nullptr)
	{
		return -1.0f;
	}

	return (float)StatInfo->exp() / static_cast<FPlayerStatData*>(StatData)->MaxExp;
}

FBaseStatData* UStatComponent::GetStatData()
{
	Protocol::ECharacterType Type = OwnerCreature->GetObjectInfo()->objtype();
	auto GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	switch (Type)
	{
		case Protocol::WARRIOR:
			return GInstance->GetDataMgr()->GetWarriorStatData(StatInfo->level());
			break;
		case Protocol::ROGUE:
			return GInstance->GetDataMgr()->GetRogueStatData(StatInfo->level());
			break;
		case Protocol::WIZARD:
			return GInstance->GetDataMgr()->GetWizardStatData(StatInfo->level());
			break;
		case Protocol::MONSTER:
			return GInstance->GetDataMgr()->GetMonsterStatData(Cast<AMonster>(OwnerCreature)->GetTemplateId());
			break;
		default:
			return nullptr;
			break;

	}
	return nullptr;
}

void UStatComponent::SetStatInfo(const Protocol::StatInfo& NewStatInfo)
{
	// 1. 새로운 내 정보 씌우기
	if (StatInfo == nullptr)
	{
		StatInfo = new Protocol::StatInfo();
	}
	
	// temp 레벨업
	StatInfo->CopyFrom(NewStatInfo);

	// 2. UI 갱신하기
	Refresh();

}



