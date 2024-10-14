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

	// 1. DB���� ���� ���� üũ
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
	// �÷��̾� ����
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	// ���ٸ� ����� ���� �� �����͸� ������� �ʰڴ�
	// why? ���ٷ� �����Ϳ� �Լ��� ���ϰ� �ִ� ���� ���� �����Ͱ� ���󰡸� �ȵȴ�.
	// ����Ʈ �����͸� ����Ͽ� ��ü ������ �Ѱ��ָ� ���۷��� ī��Ʈ�� �����ϱ� ������ ��·�� �� ���� ������
	// ��ü�� �Ҹ����� ����
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
	// ���� üũ : Ŭ���̾�Ʈ���� ���� ������ ������ Validation üũ�� �ϰ� �Ѿ�� ��
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
