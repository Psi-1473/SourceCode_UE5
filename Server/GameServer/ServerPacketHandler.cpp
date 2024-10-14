#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "ObjectUtils.h"
#include "DBManager.h"
#include "Room.h"
#include "GameSession.h"
#include "Player.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	//Protocol::S_LOGIN loginPkt;

	// 1. DB에서 유저 정보 체크
	GDBManager->DoAsync(&DBManager::HandleLogin, pkt.id(), session);

	return true;
}

bool Handle_C_CHECKNAME(PacketSessionRef& session, Protocol::C_CHECKNAME& pkt)
{
	GDBManager->DoAsync(&DBManager::CheckName, session, pkt.name());

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// 플레이어 생성
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	// 람다를 사용할 때는 생 포인터를 사용하지 않겠다
	// why? 람다로 데이터와 함수를 지니고 있는 동안 관련 데이터가 날라가면 안된다.
	// 스마트 포인터를 사용하여 객체 변수를 넘겨주면 레퍼런스 카운트가 증가하기 때문에 어쨌든 다 쓰기 전까진
	// 객체가 소멸하지 않음
	std::cout<< " Enter Game " << std::endl;
	GRoom->DoAsync(&Room::SetEnterPlayerInfo, player, pkt.playerid());

	return true;
}

bool Handle_C_LOAD_INFO(PacketSessionRef& session, Protocol::C_LOAD_INFO& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::SetPlayerComponentInfo, player);

	return true;
}

bool Handle_C_CREATECHARACTER(PacketSessionRef& session, Protocol::C_CREATECHARACTER& pkt)
{
	GDBManager->DoAsync(&DBManager::CreateCharacter, session, pkt);

	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::HandleLeavePlayer, player);
	//room->HandleLeavePlayer(player);

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);
	
	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;
	
	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;
	
	// TODO
	// 예외 체크 : 클라이언트에서 보낸 정보는 무조건 Validation 체크를 하고 넘어가야 함
	//room->HandleMove(pkt);
	room->DoAsync(&Room::HandleMove, pkt);

	return true;
}

bool Handle_C_ATTACK(PacketSessionRef& session, Protocol::C_ATTACK& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleAttack, pkt);
	return true;
}

bool Handle_C_USESKILL(PacketSessionRef& session, Protocol::C_USESKILL& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleUseSkill, player, pkt);
	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}

bool Handle_C_SETGOLD(PacketSessionRef& session, Protocol::C_SETGOLD& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleSetGold, pkt, player);
	return true;
}

bool Handle_C_ADDITEM(PacketSessionRef& session, Protocol::C_ADDITEM& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleAddItem, pkt, player);
	return true;
}

bool Handle_C_ITEMSWAP(PacketSessionRef& session, Protocol::C_ITEMSWAP& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleSwapItem, pkt);
	return true;
}

bool Handle_C_SET_QUICK(PacketSessionRef& session, Protocol::C_SET_QUICK& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleQuickSlot, pkt, player);
	return true;
}

bool Handle_C_EQUIP(PacketSessionRef& session, Protocol::C_EQUIP& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleEquipItem, pkt, player);
	return true;
}

bool Handle_C_UNEQUIP(PacketSessionRef& session, Protocol::C_UNEQUIP& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleUnEquipItem, pkt, player);
	return true;
}

bool Handle_C_USEITEM(PacketSessionRef& session, Protocol::C_USEITEM& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleUseItem, pkt, player);
	return true;
}

bool Handle_C_REMOVEITEM(PacketSessionRef& session, Protocol::C_REMOVEITEM& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleRemoveItem, pkt, player);
	return true;
}

bool Handle_C_UPGRADESKILL(PacketSessionRef& session, Protocol::C_UPGRADESKILL& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleUpgradeSkill, pkt, player);
	return true;
}

bool Handle_C_QUEST(PacketSessionRef& session, Protocol::C_QUEST& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleQuest, pkt, player);
	return true;
}

bool Handle_C_RENEW_TARGET(PacketSessionRef& session, Protocol::C_RENEW_TARGET& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);
	
	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;
	
	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;
	
	room->DoAsync(&Room::HandleRenewTarget, pkt, player);
	return true;
}

bool Handle_C_QUICK_QUEST(PacketSessionRef& session, Protocol::C_QUICK_QUEST& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleQuickQuest, pkt, player);
	return true;
}

bool CheckValidation(PacketSessionRef& session)
{
	return false;
}
