// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/ShopSlotWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "DataManager.h"
#include "Data/DataStruct.h"
#include "Popup/ShopWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlockImageDecorator.h"

// temp
#include "MyPlayer.h"
#include "ActorComponents/InventoryComponent.h"
void UShopSlotWidget::Init(Protocol::ENpcType NpcType, int ItemId, TObjectPtr<UMyGameInstance> Instance)
{
	TemplateId = ItemId;
	GInstance = Instance;
	switch (NpcType)
	{
	case Protocol::NPC_WEAPON:
		SetWeapon(ItemId);
		break;
	case Protocol::NPC_ARMOR:
		SetArmor(ItemId);
		break;
	case Protocol::NPC_CONSUMABLE:
		SetConsumable(ItemId);
		break;
	}

	Btn_Buy->OnClicked.AddDynamic(this, &UShopSlotWidget::Click_Buy);
}

void UShopSlotWidget::Click_Buy()
{
	UE_LOG(LogTemp, Warning, TEXT("Click! Buy"));

	if (GInstance->GetPlayer()->GetInventory()->GetGold() < BuyPrice)
	{
		// 구매 불가
		return;
	}


	Protocol::C_ADDITEM Pkt;
	{
		Pkt.set_templateid(TemplateId);
		Pkt.set_gettype(Protocol::GET_BUY);
		Pkt.set_itemtype(ItemType);
		Pkt.set_count(1);

		SEND_PACKET(Pkt);
	}

	
}


void UShopSlotWidget::SetWeapon(int ItemId)
{
	FWeaponData* Data = GInstance->GetDataMgr()->GetWeaponData(ItemId);
	BuyPrice = Data->BuyPrice;
	ItemType = Protocol::ITEM_WEAPON;
	Text_ItemName->SetText(FText::FromString(Data->ItemName));
	Text_Price->SetText(FText::FromString(FString::FromInt(Data->BuyPrice)));
	Img_Item->SetBrush(GInstance->GetDataMgr()->GetWeaponImage(ItemId)->Brush);
}

void UShopSlotWidget::SetArmor(int ItemId)
{
	FArmorData* Data = GInstance->GetDataMgr()->GetArmorData(ItemId);
	BuyPrice = Data->BuyPrice;
	ItemType = Protocol::ITEM_ARMOR;
	Text_ItemName->SetText(FText::FromString(Data->ItemName));
	Text_Price->SetText(FText::FromString(FString::FromInt(Data->BuyPrice)));
	Img_Item->SetBrush(GInstance->GetDataMgr()->GetArmorImage(ItemId)->Brush);
}

void UShopSlotWidget::SetConsumable(int ItemId)
{
	FConsumableData* Data = GInstance->GetDataMgr()->GetConsumableData(ItemId);
	BuyPrice = Data->BuyPrice;
	ItemType = Protocol::ITEM_CONSUMABLE;
	Text_ItemName->SetText(FText::FromString(Data->ItemName));
	Text_Price->SetText(FText::FromString(FString::FromInt(Data->BuyPrice)));
	Img_Item->SetBrush(GInstance->GetDataMgr()->GetConsumableImage(ItemId)->Brush);
}


