// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/InventoryWidget.h"
#include "MyGameInstance.h"
#include "SubItem/InvenSlotWidget.h"
#include "Popup/ShopWidget.h"
#include "UIManager.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "ActorComponents/InventoryComponent.h"

void UInventoryWidget::Init(TObjectPtr<UInventoryComponent> NewInventory, TWeakObjectPtr<UShopWidget> NewShopWidget)
{
	Inventory = NewInventory;
	if (NewShopWidget != nullptr)
	{
		bShopMode = true;
		ShopWidget = NewShopWidget;
	}

	CreateSlots();
	SetEquipment();
	Text_Gold->SetText(FText::FromString(FString::FromInt(Inventory->GetGold())));
	ItemType = Protocol::ITEM_WEAPON;
	Btn_Exit->OnClicked.AddDynamic(this, &UInventoryWidget::CloseThisUI);
	Btn_Equip->OnClicked.AddDynamic(this, &UInventoryWidget::SetEquipment);
	Btn_Consumable->OnClicked.AddDynamic(this, &UInventoryWidget::SetConsumable);
}

void UInventoryWidget::AddChildWidget(TObjectPtr<UUserWidget> Widget)
{
	Canvas->AddChild(Widget);
}

void UInventoryWidget::CloseThisUI()
{
	TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	GInstance->GetUIMgr()->CloseUI(TEXT("Inventory"));
}

void UInventoryWidget::SetEquipment()
{
	ItemType = Protocol::ITEM_WEAPON;
	for (int i = 0; i < SLOT_COUNT * SLOT_ROW_COUNT; i++)
	{
		InvenSlots[i]->SetItem(Inventory->GetEquipments()[i]);
	}
}

void UInventoryWidget::SetConsumable()
{
	ItemType = Protocol::ITEM_CONSUMABLE;
	for (int i = 0; i < SLOT_COUNT * SLOT_ROW_COUNT; i++)
	{
		InvenSlots[i]->SetItem(Inventory->GetConsumables()[i]);
	}
}

void UInventoryWidget::Refresh()
{
	if(ItemType == Protocol::ITEM_CONSUMABLE)
	{
		SetConsumable();
	}
	else
	{
		SetEquipment();
	}

	Text_Gold->SetText(FText::FromString(FString::FromInt(Inventory->GetGold())));
}

bool UInventoryWidget::GetIsShopMode()
{
	if(ShopWidget == nullptr)
	{
		return false;
	}

	if(ShopWidget->GetShopMode() == SHOP_BUY)
	{
		return false;
	}

	return true;
}

void UInventoryWidget::CreateSlots()
{

	TArray<TObjectPtr<class UHorizontalBox>> Slots;
	Slots.Add(SlotRow);
	Slots.Add(SlotRow_1);
	Slots.Add(SlotRow_2);
	Slots.Add(SlotRow_3);
	Slots.Add(SlotRow_4);
	Slots.Add(SlotRow_5);

	for (int i = 0; i < SLOT_COUNT * SLOT_ROW_COUNT; i++)
	{
		TObjectPtr<UMyGameInstance> GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
		TObjectPtr<UInvenSlotWidget> Widget = Cast<UInvenSlotWidget>(GInstance->GetUIMgr()->CreateSubItem(InvenSlotClass));
		Widget->Init(GInstance, this, i);
		int Index = i / SLOT_ROW_COUNT;
		Slots[Index]->AddChild(Widget);
		InvenSlots.Add(Widget);
	}
}
