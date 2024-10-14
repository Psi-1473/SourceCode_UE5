// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/LoginWidget.h"
#include "Protocol.pb.h"
#include "MyGameInstance.h"
#include "MyRpg.h"
#include "Components/EditableText.h"
#include "Components/Button.h"

void ULoginWidget::NativeConstruct()
{
	Btn_Login->OnClicked.AddDynamic(this, &ULoginWidget::Login);
}

void ULoginWidget::Login()
{
	// Temp
	// id로 일단 로그인 시도
	// 해당 id가 있다면 기존 데이터 불러오고 없으면 새로 생성해서 들어가도록 하자
	if (EText_Id->Text.ToString().Len() <= 2)
	{
		return;
	}
	else
	{
		Protocol::C_LOGIN Pkt;
		Pkt.set_id(TCHAR_TO_UTF8(*EText_Id->Text.ToString()));
		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
		Cast<UMyGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
	}
}
