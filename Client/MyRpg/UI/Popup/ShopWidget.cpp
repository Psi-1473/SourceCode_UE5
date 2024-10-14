// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/ShopWidget.h"
#include "Npc.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "MyPlayer.h"
#include "ActorComponents/InventoryComponent.h"
#include "Item.h"

#include "Popup/InventoryWidget.h"
#include "SubItem/InvenSlotWidget.h"
#include "SubItem/ShopSlotWidget.h"
#include "SubItem/SellSlotWidget.h"

void UShopWidget::Init(TWeakObjectPtr<ANpc> NewNpc, TObjectPtr<UMyGameInstance> Instance)
{
	OwnerNpc = NewNpc;
	GInstance = Instance;

	FString ItemList = OwnerNpc->GetNpcData()->Items;
	Protocol::ENpcType NpcType = (Protocol::ENpcType)OwnerNpc->GetNpcData()->NpcType;

	TArray<FString> Items;
	ItemList.ParseIntoArray(Items, TEXT(" "), true);
	
	for (const auto& Item : Items)
	{
		int32 ItemId = FCString::Atoi(*Item);
		TObjectPtr<UShopSlotWidget> Widget = Cast<UShopSlotWidget>(GInstance->GetUIMgr()->CreateSubItem(BuySlotClass));
		Scroll_BuyScroll->AddChild(Widget);
		Widget->Init(NpcType, ItemId, GInstance);
	}

	SetBuyMode();
	InvenWidget->Init(GInstance->GetPlayer()->GetInventory(), this);
	Btn_BuyTap->OnClicked.AddDynamic(this, &UShopWidget::SetBuyMode);
	Btn_SellTap->OnClicked.AddDynamic(this, &UShopWidget::SetSellMode);
	Btn_Exit->OnClicked.AddDynamic(this, &UShopWidget::CloseThisUI);
	Btn_Sell->OnClicked.AddDynamic(this, &UShopWidget::SellItems);
}

void UShopWidget::SetBuyMode()
{
	Mode = SHOP_BUY;
	SellTap->SetVisibility(ESlateVisibility::Hidden);
	BuyTap->SetVisibility(ESlateVisibility::Visible);
}

void UShopWidget::SetSellMode()
{
	Mode = SHOP_SELL;
	SellTap->SetVisibility(ESlateVisibility::Visible);
	BuyTap->SetVisibility(ESlateVisibility::Hidden);
}

void UShopWidget::CloseThisUI()
{
	for (const auto& Item : ItemsToBeSold)
	{
		Item.Value->SetToBeSold(false);
	}
	GInstance->GetUIMgr()->CloseUI(TEXT("Shop"));
}

void UShopWidget::SellItems()
{
	if (ItemsToBeSold.Num() <= 0)
	{
		return;
	}

	Protocol::C_REMOVEITEM Pkt;
	Protocol::C_SETGOLD GoldPkt;
	TArray<uint64> Keys;
	int32 SellGold = 0;
	for (const auto& Item : ItemsToBeSold)
	{
		Pkt.add_itemid(Item.Key);
		Pkt.add_removecount(Item.Value->GetCountToBeSold());
		SellGold += Item.Value->GetItemData()->SellPrice;
	}
	Pkt.set_bsell(true);
	GoldPkt.set_gold(SellGold);
	{
		SEND_PACKET(Pkt);
	}
	{
		SEND_PACKET(GoldPkt);
	}

	for (const auto& Item : SellSlots)
	{
		Item.Value->RemoveFromViewport();
	}
	SellSlots.Empty();
	ItemsToBeSold.Empty();
}

void UShopWidget::Refresh()
{
	InvenWidget->Refresh();

	int32 SellGold = 0;
	for (const auto& Item : ItemsToBeSold)
	{
		SellGold += Item.Value->GetItemData()->SellPrice;
	}

	Text_Total->SetText(FText::FromString(FString::FromInt(SellGold)));
}

void UShopWidget::SetItemToBeSold(TObjectPtr<UItem> Item)
{
	// 1. 판매할 아이템 리스트에 아이템 정보 추가
	ItemsToBeSold.Add(Item->GetDbId(), Item);

	// 2. 판매 예정 아이템 UI인 SellSlotUI 생성
	auto Widget = GInstance->GetUIMgr()->CreateSubItem(SellSlotClass);
	TObjectPtr<USellSlotWidget> SellSlot = Cast<USellSlotWidget>(Widget);
	
	SellSlot->Init(Item, this);
	Scroll_SellScroll->AddChild(SellSlot);
	SellSlots.Add(Item->GetDbId(), SellSlot);
	Refresh();
}

void UShopWidget::RemoveItemToBeSold(TObjectPtr<UItem> Item)
{
	uint64 DbId = Item->GetDbId();
	Item->SetToBeSold(false);
	Item->SetCountToBeSold(0);
	ItemsToBeSold.Remove(DbId);
	SellSlots[DbId]->RemoveFromViewport();
	SellSlots.Remove(DbId);

	GInstance->GetUIMgr()->RefreshUI(TEXT("Shop"));
}
