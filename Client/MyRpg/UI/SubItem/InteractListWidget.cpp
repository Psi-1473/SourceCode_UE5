// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/InteractListWidget.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "UIManager.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Popup/ShopWidget.h"
#include "Popup/NpcQuestWidget.h"
#include "Npc.h"


void UInteractListWidget::Init(InteractUIType InitType, TWeakObjectPtr<ANpc> Npc, TObjectPtr<UMyGameInstance> Instance)
{
	// 1. 타입이 뭐냐에 따라 이미지 변경
	OwnerNpc = Npc;
	GInstance = Instance;
	FRichImageRow* Image = GInstance->GetDataMgr()->GetInteractListImage((int)InitType);
	Img_ItemBackground->SetBrush(Image->Brush);

	// 2. 텍스트와 클릭 시 실행할 함수 변경
	switch (InitType)
	{
		case InteractUIType::SHOP:
			SetShopType();
			break;
		case InteractUIType::QUEST:
			SetQuestType();
			break;
		case InteractUIType::EXIT:
			SetExitType();
			break;
	}

	Btn_Click->OnClicked.AddDynamic(this, &UInteractListWidget::OnMenuClicked);
}

void UInteractListWidget::OnMenuClicked()
{
	if (OnButtonClicked)
	{
		(this->*OnButtonClicked)();
	}
}

void UInteractListWidget::SetQuestType()
{
	FString Str = TEXT("퀘스트");
	FText Text = FText::FromString(Str);
	Text_Name->SetText(Text);
	OnButtonClicked = &UInteractListWidget::PopupQuest;
}

void UInteractListWidget::SetShopType()
{
	FString Str = TEXT("상점");
	FText Text = FText::FromString(Str);
	Text_Name->SetText(Text);
	OnButtonClicked = &UInteractListWidget::PopupShop;
}

void UInteractListWidget::SetExitType()
{
	FString Str = TEXT("나가기");
	FText Text = FText::FromString(Str);
	Text_Name->SetText(Text);
	OnButtonClicked = &UInteractListWidget::Exit;
}

void UInteractListWidget::PopupQuest()
{
	TObjectPtr<UNpcQuestWidget> Widget = Cast<UNpcQuestWidget>(GInstance->GetUIMgr()->PopupUI(TEXT("NpcQuest")));
	Widget->Init(OwnerNpc, GInstance);

}

void UInteractListWidget::PopupShop()
{
	TObjectPtr<UShopWidget> Widget = Cast<UShopWidget>(GInstance->GetUIMgr()->PopupUI(TEXT("Shop")));
	Widget->Init(OwnerNpc, GInstance);
}

void UInteractListWidget::Exit()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("Npc"));
}

