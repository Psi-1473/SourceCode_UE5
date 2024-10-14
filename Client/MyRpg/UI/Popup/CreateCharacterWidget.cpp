// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/CreateCharacterWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "MyGameInstance.h"

#include "UIManager.h"
void UCreateCharacterWidget::NativeConstruct()
{
	SetVisibilityHidden();
	GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	Btn_Warrior->OnClicked.AddDynamic(this, &UCreateCharacterWidget::SelectWarrior);
	Btn_Rogue->OnClicked.AddDynamic(this, &UCreateCharacterWidget::SelectRogue);
	Btn_Wizard->OnClicked.AddDynamic(this, &UCreateCharacterWidget::SelectWizard);
	Btn_Create->OnClicked.AddDynamic(this, &UCreateCharacterWidget::CreateCharacter);
	Btn_Duplication->OnClicked.AddDynamic(this, &UCreateCharacterWidget::CheckDuplicate);
}

void UCreateCharacterWidget::CheckDuplicate()
{
	// 1. 닉네임 규격 체크
	if (EText_Name->Text.ToString().Len() < 2)
	{
		// 닉네임 길이가 짧다는 문장 보내기
		GInstance->GetUIMgr()->Alarm(TEXT("닉네임이 너무 짧습니다."));
		return;
	}

	// 2. 서버에 중복된 패킷인지 확인하는 테스트 보내기
	Protocol::C_CHECKNAME Pkt;
	Name = EText_Name->Text.ToString();
	Pkt.set_name(TCHAR_TO_UTF8(*Name));


	SEND_PACKET(Pkt);
}

void UCreateCharacterWidget::SelectWarrior()
{
	SetVisibilityHidden();
	Img_SelectWarrior->SetVisibility(ESlateVisibility::Visible);
	Text_Class->SetText(FText::FromString(TEXT("전사")));
	Type = Protocol::WARRIOR;
}

void UCreateCharacterWidget::SelectRogue()
{
	SetVisibilityHidden();
	Img_SelectRogue->SetVisibility(ESlateVisibility::Visible);
	Text_Class->SetText(FText::FromString(TEXT("도적")));
	Type = Protocol::ROGUE;
}

void UCreateCharacterWidget::SelectWizard()
{
	SetVisibilityHidden();
	Img_SelectWizard->SetVisibility(ESlateVisibility::Visible);
	Text_Class->SetText(FText::FromString(TEXT("마법사")));
	Type = Protocol::WIZARD;
}

void UCreateCharacterWidget::CreateCharacter()
{
	if (bDuplication == true)
	{
		// 닉네임 중복확인 하라는 안내문 띄우기
		GInstance->GetUIMgr()->Alarm(TEXT("닉네임 중복 확인을 해주십시오."));
		return;
	}

	if (Type == Protocol::NONE)
	{
		// 직업을 선택하라는 안내문 띄우기
		GInstance->GetUIMgr()->Alarm(TEXT("클래스를 선택해 주십시오."));
		return;
	}

	Protocol::C_CREATECHARACTER Pkt;
	Pkt.set_userid(GInstance->UserDBId);
	Pkt.set_type(Type);
	Pkt.set_name(TCHAR_TO_UTF8(*Name));

	SEND_PACKET(Pkt);
	// 패킷 보내기
}

void UCreateCharacterWidget::SetDuplication(bool isDuplicated)
{
	bDuplication = isDuplicated;
	if (bDuplication == true)
	{
		GInstance->GetUIMgr()->Alarm(TEXT("이미 생성된 닉네임입니다."));
		return;
	}

	GInstance->GetUIMgr()->Alarm(TEXT("사용 가능한 닉네임 입니다."));
}

void UCreateCharacterWidget::SetVisibilityHidden()
{
	Img_SelectWarrior->SetVisibility(ESlateVisibility::Hidden);
	Img_SelectRogue->SetVisibility(ESlateVisibility::Hidden);
	Img_SelectWizard->SetVisibility(ESlateVisibility::Hidden);
}
