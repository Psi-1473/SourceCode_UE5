// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "UIManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ActorComponents/InventoryComponent.h"
#include "ActorComponents/StatComponent.h"
#include "Item/Item.h"
#include "SubItem/EquipSlotWidget.h"

void UEquipmentWidget::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
	Refresh();

	Btn_Exit->OnClicked.AddDynamic(this, &UEquipmentWidget::CloseThisUI);
}

void UEquipmentWidget::Refresh()
{
	TArray<TObjectPtr<UItem>> EquippedItem = GInstance->GetPlayer()->GetInventory()->GetEquippedItems();
	Protocol::StatInfo* Stat = GInstance->GetPlayer()->GetStatComponent()->GetStatInfo();
	FBaseStatData* MaxStatData = GInstance->GetPlayer()->GetStatComponent()->GetStatData();

	Helmet->Init(GInstance, EquippedItem[0]);
	Top->Init(GInstance, EquippedItem[1]);
	Bottom->Init(GInstance, EquippedItem[2]);
	Boots->Init(GInstance, EquippedItem[3]);
	Gloves->Init(GInstance, EquippedItem[4]);
	Weapon->Init(GInstance, EquippedItem[5]);

	Text_Name->SetText(FText::FromString(GInstance->GetPlayer()->GetPlayerName()));
	Text_Class->SetText(GetClassName(GInstance->GetPlayer()));
	Text_Level->SetText(FText::FromString(FString::FromInt(Stat->level())));
	Text_Atk->SetText(FText::FromString(FString::FromInt(Stat->atk())));
	Text_Dfs->SetText(FText::FromString(FString::FromInt(Stat->dfs())));
	Text_Hp->SetText(FText::FromString(FString::FromInt(MaxStatData->MaxHp)));
	Text_Mp->SetText(FText::FromString(FString::FromInt(MaxStatData->MaxMp)));
	


}

void UEquipmentWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("Equipment"));
}

FText UEquipmentWidget::GetClassName(TWeakObjectPtr<class AMyPlayer> Player)
{
	switch (Player->GetObjectInfo()->objtype())
	{
		case Protocol::WARRIOR:
			return FText::FromString(TEXT("워리어"));
			break;
		case Protocol::ROGUE:
			return FText::FromString(TEXT("도적"));
			break;
		case Protocol::WIZARD:
			return FText::FromString(TEXT("마법사"));
			break;
		default:
			return FText::FromString(TEXT(""));
			break;
	}
}
