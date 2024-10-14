// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/CharacterSlotWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Popup/SelectCharacterWidget.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "SceneManager.h"

void UCharacterSlotWidget::SetParentWidget(TWeakObjectPtr<USelectCharacterWidget> Widget, TObjectPtr<UMyGameInstance> Instance)
{
	Parent = Widget;
	GInstance = Instance;
	DeSelectThisSlot();
}

void UCharacterSlotWidget::SetCharacter(const Protocol::BaseCharacterInfo& Info)
{
	// 이 슬롯을 특정 캐릭터 정보로 채우기
	Character->SetVisibility(ESlateVisibility::Visible);
	Empty->SetVisibility(ESlateVisibility::Hidden);

	CharacterId = Info.object_id();
	Btn_Select->OnClicked.AddDynamic(this, &UCharacterSlotWidget::SelectThisSlot);
	
	UE_LOG(LogTemp, Warning, TEXT("Set Character!"));
	Text_NickName->SetText(FText::FromString(FString(UTF8_TO_TCHAR(Info.name().c_str()))));
	Text_Level->SetText(FText::FromString(FString::FromInt(Info.level())));
	Text_Class->SetText(FText::FromString(GetClassName(Info.type())));
	Img_Character->SetBrush(GInstance->GetDataMgr()->GetClassPortraitImage(Info.type())->Brush);
	Img_Class->SetBrush(GInstance->GetDataMgr()->GetClassIconImage(Info.type())->Brush);

}

void UCharacterSlotWidget::SetEmpty()
{
	// 캐릭터 슬롯 비우기
	Character->SetVisibility(ESlateVisibility::Hidden);
	Empty->SetVisibility(ESlateVisibility::Visible);

	Btn_Select->OnClicked.Clear();
}

void UCharacterSlotWidget::SelectThisSlot()
{
	// 현재 슬롯을 선택된 상태로 만들기
	Parent->SetSlotsDeselected();
	Parent->SetSelectedSlot(this);
	Img_Selected->SetVisibility(ESlateVisibility::Visible);
}

void UCharacterSlotWidget::DeSelectThisSlot()
{
	Img_Selected->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSlotWidget::EnterGameWithThisSlot()
{
	GInstance->CharacterDBId = CharacterId;
	GInstance->GetSceneMgr()->LoadSceneAsync(ESceneType::GAME);
	
}

FString UCharacterSlotWidget::GetClassName(Protocol::ECharacterType Type)
{
	FString Name;
	
	switch (Type)
	{
		case Protocol::WARRIOR:
			Name = TEXT("전 사");
			break;
		case Protocol::ROGUE:
			Name = TEXT("도 적");
			break;
		case Protocol::WIZARD:
			Name = TEXT("마법사");
			break;
		default:
			break;
	}
	return Name;
}
