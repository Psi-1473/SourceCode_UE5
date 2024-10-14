// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct PacketHeader
{
	uint16 size;
	uint16 id; // ��������ID (ex. 1=�α���, 2=�̵���û)
};

class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);

private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};


/*
	����
*/

//class MYRPG_API MyRpgbarModule : public IModuleInterface
//{
//public:
//	virtual void StartupModule() override;
//	virtual void ShutdownModule() override;
//
//private:
//	void AddMenuExtension(FMenuBuilder& MenuBuilder);
//	void OnExportHeightMap();
//};


/*
	��ũ��
*/



#define USING_SHARED_PTR(name)	using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);

#include "ClientPacketHandler.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Protocol.pb.h"

using namespace Protocol;

#define SEND_PACKET(Pkt)														\
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);		\
	Cast<UMyGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);