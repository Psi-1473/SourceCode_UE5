// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/ItemInfoWidget.h"
#include "Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "MyGameInstance.h"
#include "Protocol.pb.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "InventoryWidget.h"
#include "UIManager.h"
#include "Data/DataStruct.h"
#include "MyPlayer.h"

void UItemInfoWidget::Init(TObjectPtr<UItem> BaseItem, TObjectPtr<UUserWidget> BaseWidget, FString WidgetType, TObjectPtr<UMyGameInstance> Instance)
{
	//auto Inventory = Cast<UInventoryWidget>(Instance->GetUIMgr()->GetPopupUI("Inventory"));

	// 1. 위젯 위치 설정하기


	// 2. 아이템 정보 세팅
	Item = BaseItem;

	RText_ItemName->SetText(FText::FromString(Item->GetItemData()->ItemName));
	RText_ItemDesc->SetText(FText::FromString(Item->GetItemData()->ItemDesc));
	Text_Power->SetText(FText::FromString(FString::FromInt(Item->GetItemData()->Power)));
	Img_Item->SetBrush(Item->GetItemImage()->Brush);
	



/*  =========== Grade랑 Power Type 텍스트 설정은 임시로 이렇게 하고 나중에 수정하기 ===========*/
	// grade
	FString Grade;
	if(Item->GetItemData()->Grade == 0)
	{
		RText_ItemGrade->SetText(FText::FromString(TEXT("노말")));
	}
	else
	{
		RText_ItemGrade->SetText(FText::FromString(TEXT("레어")));
	}

	// powertype
	if (Item->GetItemType() == Protocol::ITEM_WEAPON)
	{
		Text_PowerType->SetText(FText::FromString(TEXT("공격력")));
	}
	else if (Item->GetItemType() == Protocol::ITEM_ARMOR)
	{
		Text_PowerType->SetText(FText::FromString(TEXT("방어력")));
	}
	else if (Item->GetItemType() == Protocol::ITEM_CONSUMABLE)
	{
		Text_PowerType->SetText(FText::FromString(TEXT("회복량")));
	}

	if (Item->GetClass() == Protocol::WARRIOR)
	{
		Text_ClassType->SetText(FText::FromString(TEXT("워리어 착용 가능")));
	}
	else if (Item->GetClass() == Protocol::ROGUE)
	{
		Text_ClassType->SetText(FText::FromString(TEXT("도적 착용 가능")));
	}
	else if (Item->GetClass() == Protocol::WIZARD)
	{
		Text_ClassType->SetText(FText::FromString(TEXT("마법사 착용 가능")));
	}

	if (Item->GetUsableItem(Instance->GetPlayer().Get()) == true)
	{
		// 텍스트 칼라 바꾸기
	}
	else
	{
		// 텍스트 칼라 바꾸기
	}
}
