// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/SetItemCountWidget.h"
#include "Item.h"
#include "ShopWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Components/EditableText.h"
#include "Components/Button.h"

void USetItemCountWidget::Init(TObjectPtr<UMyGameInstance> Instance,  TObjectPtr<UItem> NewItem, TWeakObjectPtr<UShopWidget> NewShopWidget)
{
	Item = NewItem;
	ShopWidget = NewShopWidget;
	GInstance = Instance;

	Btn_Exit->OnClicked.AddDynamic(this, &USetItemCountWidget::CloseThisUI);
	Btn_Confirm->OnClicked.AddDynamic(this, &USetItemCountWidget::Confirm);
}

void USetItemCountWidget::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("SetItemCount"));
}

void USetItemCountWidget::Confirm()
{
	FString InputValue = EText_Input->GetText().ToString();
	int32 InputNumber = FCString::Atoi(*InputValue);

	if (InputNumber > Item->GetCount())
	{
		// 불가. 나중에 경고문 추가
		return;
	}

	Item->SetCountToBeSold(InputNumber);
	ShopWidget->SetItemToBeSold(Item);

	CloseThisUI();
}
