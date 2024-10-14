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
	// id�� �ϴ� �α��� �õ�
	// �ش� id�� �ִٸ� ���� ������ �ҷ����� ������ ���� �����ؼ� ������ ����
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
