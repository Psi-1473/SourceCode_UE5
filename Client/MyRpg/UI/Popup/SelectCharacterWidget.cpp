// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/SelectCharacterWidget.h"
#include "Components/Button.h"
#include "SubItem/CharacterSlotWidget.h"

#include "MyGameInstance.h"
#include "UIManager.h"
#include "Popup/CreateCharacterWidget.h"

USelectCharacterWidget::USelectCharacterWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	

	
}

void USelectCharacterWidget::Init(TObjectPtr<class UMyGameInstance> Instance)
{
	GInstance = Instance;
	Slots.Add(CharacterSlot1);
	Slots.Add(CharacterSlot2);
	Slots.Add(CharacterSlot3);
	Btn_Create->OnClicked.AddDynamic(this, &USelectCharacterWidget::PopupCreateWidget);
	Btn_Start->OnClicked.AddDynamic(this, &USelectCharacterWidget::EnterGame);
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetParentWidget(this, GInstance);
	}
}

void USelectCharacterWidget::SetSlot(const Protocol::S_LOGIN& Pkt)
{
	int32 Index = 0;

	for (Index; Index < Pkt.characters_size(); Index++)
	{
		Protocol::BaseCharacterInfo Info = Pkt.characters().at(Index);
		Slots[Index]->SetCharacter(Info);
	}

	for (Index; Index < 3; Index++)
	{
		Slots[Index]->SetEmpty();
	}
}

void USelectCharacterWidget::SetSlotsDeselected()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->DeSelectThisSlot();
	}
}

void USelectCharacterWidget::EnterGame()
{
	if (SelectedSlot == nullptr)
	{
		return;
	}
	SelectedSlot->EnterGameWithThisSlot();
}

void USelectCharacterWidget::PopupCreateWidget()
{
	// 캐릭터 생성창 열기
	GInstance->GetUIMgr()->PopupUI(TEXT("CreateCharacter"));
}

