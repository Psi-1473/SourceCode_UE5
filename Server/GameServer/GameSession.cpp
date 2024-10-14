#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "DBManager.h"
#include "ServerPacketHandler.h"
//#include "Room.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
	std::cout << "Client Session Connected!" << std::endl;
}

void GameSession::OnDisconnected()
{
	cout << " Session Disconnect " << endl;
	GDBManager->DoAsync(&DBManager::SavePlayerInfo, player.load());
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId �뿪 üũ
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
}