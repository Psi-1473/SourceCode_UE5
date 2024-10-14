// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/SellSlotWidget.h"
#include "Item.h"
#include "Popup/ShopWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlockImageDecorator.h"

void USellSlotWidget::Init(TObjectPtr<UItem> NewItem, TWeakObjectPtr<UShopWidget> NewShopSlot)
{
	Item = NewItem;
	ShopWidget = NewShopSlot;
	SetInfo();

	Btn_Cancle->OnClicked.AddDynamic(this, &USellSlotWidget::Cancle);
}

void USellSlotWidget::SetInfo()
{
	Img_Item->SetBrush(Item->GetItemImage()->Brush);
	Text_ItemName->SetText(FText::FromString(Item->GetItemData()->ItemName));
	Text_Gold->SetText(FText::FromString(FString::FromInt(Item->GetItemData()->SellPrice)));

	if(Item->GetCountToBeSold() > 1)
	{
		Text_Count->SetText(FText::FromString(FString::FromInt(Item->GetCountToBeSold())));
	}


	
}

void USellSlotWidget::Cancle()
{
	ShopWidget->RemoveItemToBeSold(Item);
}
