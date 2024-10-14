// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/InvenSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Weapon.h"
#include "Item/Armor.h"
#include "Item/Consumable.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "InputCoreTypes.h"
#include "Popup/ItemInfoWidget.h"
#include "Popup/InventoryWidget.h"
#include "Popup/ShopWidget.h"
#include "Popup/SetItemCountWidget.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "DragWidget/SlotDrag.h"
#include "MyPlayer.h"
#include "ActorComponents/InventoryComponent.h"

UInvenSlotWidget::UInvenSlotWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UInvenSlotWidget::Init(TObjectPtr<UMyGameInstance> Instance, TWeakObjectPtr<UInventoryWidget> Widget, int32 Index)
{
	GInstance = Instance;
	InventoryWidget = Widget;
	SlotIndex = Index;
}


void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInvenSlotWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);
	if (GInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instance NUll"));
		return;
	}
	if (Item == nullptr)
	{
		return;
	}
	auto Widget = Cast<UItemInfoWidget>(GInstance->GetUIMgr()->PopupUI("ItemInfo"));
	Widget->Init(Item, this, TEXT("Inventory"), GInstance);
}

void UInvenSlotWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);
	if (GInstance == nullptr)
	{
		return;
	}
	
	GInstance->GetUIMgr()->CloseUI("ItemInfo");
}

FReply UInvenSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" MOUSE DOWN "));
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (Item != nullptr)
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			UE_LOG(LogTemp, Warning, TEXT("Drag Start"));
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		if (GetBaseInventory()->GetIsShopMode() && Item != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Sell Register "));
			if (Item->GetToBeSold() == true)
			{
				return Reply.NativeReply;
			}
			Item->SetToBeSold(true);
			if (Item->GetItemType() == Protocol::ITEM_CONSUMABLE)
			{
				auto Widget = Cast<USetItemCountWidget>(GInstance->GetUIMgr()->PopupUI(TEXT("SetItemCount")));
				Widget->Init(GInstance, Item, InventoryWidget->GetShopWidget());
			}
			else
			{
				InventoryWidget->GetShopWidget()->SetItemToBeSold(Item);
			}
		}
	}

	return Reply.NativeReply;
}

void UInvenSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if (OutOperation == nullptr)
	{
		TObjectPtr<USlotDrag> DragOper = NewObject<USlotDrag>();
		OutOperation = DragOper;
		DragOper->Item = Item;

		if (DragVisualClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dragging"));
			TObjectPtr<UInvenSlotWidget> Visual = CreateWidget<UInvenSlotWidget>(GInstance->GetWorld(), DragVisualClass);
			Visual->SetItem(Item);
			DragOper->DefaultDragVisual = Visual;
		}
	}
}

bool UInvenSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	TObjectPtr<USlotDrag> DragOper = Cast<USlotDrag>(InOperation);

	if (DragOper != nullptr)
	{
		// 스왑
		UE_LOG(LogTemp, Warning, TEXT("Drag Success"));
		GInstance->GetPlayer()->GetInventory()->SwapItem(DragOper->Item->GetItemType(), DragOper->Item->GetSlotIndex(), SlotIndex, InventoryWidget);
		return true;
	}
	else
	{
		// 응
		UE_LOG(LogTemp, Warning, TEXT("Drag Failed"));
		return false;
	}
}


FReply UInvenSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(MyGeometry, MouseEvent);

	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" LEFT MOUSE Double Click "));
		if (Item != nullptr)
		{
			Item->UseItem(GInstance);
		}
	}


	return Reply.NativeReply;
}




void UInvenSlotWidget::SetItem(TObjectPtr<class UItem> NewItem)
{
	// 1. Item값 설정
	Item = NewItem;

	if (Item == nullptr)
	{
		SetNull();
		return;
	}

	SetInfo();

}

void UInvenSlotWidget::SetInfo()
{
	Img_Item->SetVisibility(ESlateVisibility::Visible);
	//1. 텍스트 설정
	if(Item->GetCount() <= 1)
	{
		Text_Number->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		Text_Number->SetText(FText::FromString(FString::FromInt(Item->GetCount())));
	}

	// 2. 이미지 설정
	Img_Item->SetBrush(Item->GetItemImage()->Brush);

	if (Item->GetToBeSold())
	{
		UE_LOG(LogTemp, Warning, TEXT("To be Sold"));
		FSlateColor Color = FColor(255.f, 255.f, 255.f, 125.f);
		Img_ToBeSold->SetBrushTintColor(Color);
	}
	else
	{
		FSlateColor Color = FColor(255.f, 255.f, 255.f, 0.f);
		Img_ToBeSold->SetBrushTintColor(Color);
	}
}

void UInvenSlotWidget::SetNull()
{
	Text_Number->SetText(FText::FromString(TEXT("")));
	Img_Item->SetVisibility(ESlateVisibility::Hidden);
	FSlateColor Color = FColor(0.f, 0.f, 0.f, 0.f);
	Img_ToBeSold->SetBrushTintColor(Color);
}
