﻿#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
//#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Job.h"
#include "Protocol.pb.h"


#include "BoxCollider.h"
#include "SphereCollider.h"
#include "MapManager.h"
#include "DBManager.h"
#include "Room.h"
enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

void DBJob()
{
	while (true)
	{
		GDBManager->Execute();
		this_thread::sleep_for(0.1s);
	}
}

int main()
{
	ServerPacketHandler::Init();

	std::cout << "Start Server ! " << std::endl;

	GMapManager->LoadHeightMap();
	GMapManager->LoadMovableMap();
	GDBManager->ConnectDB();
	GMapManager->InitializeMobInfo();

	ServerServiceRef service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[=]() { return make_shared<GameSession>(); }, // TODO : SessionManager 등
		100);
	GRoom->CreateSpawner();
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	GThreadManager->Launch([]()
		{
			DBJob();
		});


	while (true)
	{
		this_thread::sleep_for(0.1s);
	}

	GThreadManager->Join();
}