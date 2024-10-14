#pragma once
#include "Protocol.pb.h"

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "MyRpg.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_CHECKNAME = 1002,
	PKT_S_CHECKNAME = 1003,
	PKT_C_ENTER_GAME = 1004,
	PKT_C_LOAD_INFO = 1005,
	PKT_C_CREATECHARACTER = 1006,
	PKT_S_ENTER_GAME = 1007,
	PKT_S_LOAD_SKILL = 1008,
	PKT_S_LOAD_QUICK = 1009,
	PKT_C_LEAVE_GAME = 1010,
	PKT_S_LEAVE_GAME = 1011,
	PKT_S_SPAWN = 1012,
	PKT_S_DESPAWN = 1013,
	PKT_C_MOVE = 1014,
	PKT_S_MOVE = 1015,
	PKT_C_ATTACK = 1016,
	PKT_S_ATTACK = 1017,
	PKT_C_USESKILL = 1018,
	PKT_S_USESKILL = 1019,
	PKT_S_DAMAGED = 1020,
	PKT_S_DIE = 1021,
	PKT_C_CHAT = 1022,
	PKT_S_CHAT = 1023,
	PKT_C_SETGOLD = 1024,
	PKT_S_SETGOLD = 1025,
	PKT_C_ADDITEM = 1026,
	PKT_S_ADDITEM = 1027,
	PKT_C_ITEMSWAP = 1028,
	PKT_C_SET_QUICK = 1029,
	PKT_C_EQUIP = 1030,
	PKT_S_EQUIP = 1031,
	PKT_C_UNEQUIP = 1032,
	PKT_S_SETITEMCOUNT = 1033,
	PKT_C_USEITEM = 1034,
	PKT_C_REMOVEITEM = 1035,
	PKT_S_REMOVEITEM = 1036,
	PKT_S_STATCHANGED = 1037,
	PKT_S_LEVEL_UP = 1038,
	PKT_C_UPGRADESKILL = 1039,
	PKT_S_UPGRADESKILL = 1040,
	PKT_C_QUEST = 1041,
	PKT_S_QUEST = 1042,
	PKT_C_RENEW_TARGET = 1043,
	PKT_S_LOAD_TARGET = 1044,
	PKT_C_QUICK_QUEST = 1045,
	PKT_S_QUICK_QUEST = 1046,
	PKT_S_SET_DATA = 1047,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_CHECKNAME(PacketSessionRef& session, Protocol::C_CHECKNAME& pkt);
bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);
bool Handle_C_LOAD_INFO(PacketSessionRef& session, Protocol::C_LOAD_INFO& pkt);
bool Handle_C_CREATECHARACTER(PacketSessionRef& session, Protocol::C_CREATECHARACTER& pkt);
bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt);
bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt);
bool Handle_C_ATTACK(PacketSessionRef& session, Protocol::C_ATTACK& pkt);
bool Handle_C_USESKILL(PacketSessionRef& session, Protocol::C_USESKILL& pkt);
bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt);
bool Handle_C_SETGOLD(PacketSessionRef& session, Protocol::C_SETGOLD& pkt);
bool Handle_C_ADDITEM(PacketSessionRef& session, Protocol::C_ADDITEM& pkt);
bool Handle_C_ITEMSWAP(PacketSessionRef& session, Protocol::C_ITEMSWAP& pkt);
bool Handle_C_SET_QUICK(PacketSessionRef& session, Protocol::C_SET_QUICK& pkt);
bool Handle_C_EQUIP(PacketSessionRef& session, Protocol::C_EQUIP& pkt);
bool Handle_C_UNEQUIP(PacketSessionRef& session, Protocol::C_UNEQUIP& pkt);
bool Handle_C_USEITEM(PacketSessionRef& session, Protocol::C_USEITEM& pkt);
bool Handle_C_REMOVEITEM(PacketSessionRef& session, Protocol::C_REMOVEITEM& pkt);
bool Handle_C_UPGRADESKILL(PacketSessionRef& session, Protocol::C_UPGRADESKILL& pkt);
bool Handle_C_QUEST(PacketSessionRef& session, Protocol::C_QUEST& pkt);
bool Handle_C_RENEW_TARGET(PacketSessionRef& session, Protocol::C_RENEW_TARGET& pkt);
bool Handle_C_QUICK_QUEST(PacketSessionRef& session, Protocol::C_QUICK_QUEST& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_C_CHECKNAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CHECKNAME>(Handle_C_CHECKNAME, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_LOAD_INFO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOAD_INFO>(Handle_C_LOAD_INFO, session, buffer, len); };
		GPacketHandler[PKT_C_CREATECHARACTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CREATECHARACTER>(Handle_C_CREATECHARACTER, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LEAVE_GAME>(Handle_C_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handle_C_MOVE, session, buffer, len); };
		GPacketHandler[PKT_C_ATTACK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ATTACK>(Handle_C_ATTACK, session, buffer, len); };
		GPacketHandler[PKT_C_USESKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_USESKILL>(Handle_C_USESKILL, session, buffer, len); };
		GPacketHandler[PKT_C_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CHAT>(Handle_C_CHAT, session, buffer, len); };
		GPacketHandler[PKT_C_SETGOLD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SETGOLD>(Handle_C_SETGOLD, session, buffer, len); };
		GPacketHandler[PKT_C_ADDITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ADDITEM>(Handle_C_ADDITEM, session, buffer, len); };
		GPacketHandler[PKT_C_ITEMSWAP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ITEMSWAP>(Handle_C_ITEMSWAP, session, buffer, len); };
		GPacketHandler[PKT_C_SET_QUICK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SET_QUICK>(Handle_C_SET_QUICK, session, buffer, len); };
		GPacketHandler[PKT_C_EQUIP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_EQUIP>(Handle_C_EQUIP, session, buffer, len); };
		GPacketHandler[PKT_C_UNEQUIP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_UNEQUIP>(Handle_C_UNEQUIP, session, buffer, len); };
		GPacketHandler[PKT_C_USEITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_USEITEM>(Handle_C_USEITEM, session, buffer, len); };
		GPacketHandler[PKT_C_REMOVEITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_REMOVEITEM>(Handle_C_REMOVEITEM, session, buffer, len); };
		GPacketHandler[PKT_C_UPGRADESKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_UPGRADESKILL>(Handle_C_UPGRADESKILL, session, buffer, len); };
		GPacketHandler[PKT_C_QUEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_QUEST>(Handle_C_QUEST, session, buffer, len); };
		GPacketHandler[PKT_C_RENEW_TARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_RENEW_TARGET>(Handle_C_RENEW_TARGET, session, buffer, len); };
		GPacketHandler[PKT_C_QUICK_QUEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_QUICK_QUEST>(Handle_C_QUICK_QUEST, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHECKNAME& pkt) { return MakeSendBuffer(pkt, PKT_S_CHECKNAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LOAD_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_LOAD_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LOAD_QUICK& pkt) { return MakeSendBuffer(pkt, PKT_S_LOAD_QUICK); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ATTACK& pkt) { return MakeSendBuffer(pkt, PKT_S_ATTACK); }
	static SendBufferRef MakeSendBuffer(Protocol::S_USESKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_USESKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DAMAGED& pkt) { return MakeSendBuffer(pkt, PKT_S_DAMAGED); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DIE& pkt) { return MakeSendBuffer(pkt, PKT_S_DIE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_S_CHAT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SETGOLD& pkt) { return MakeSendBuffer(pkt, PKT_S_SETGOLD); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ADDITEM& pkt) { return MakeSendBuffer(pkt, PKT_S_ADDITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_EQUIP& pkt) { return MakeSendBuffer(pkt, PKT_S_EQUIP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SETITEMCOUNT& pkt) { return MakeSendBuffer(pkt, PKT_S_SETITEMCOUNT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_REMOVEITEM& pkt) { return MakeSendBuffer(pkt, PKT_S_REMOVEITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_STATCHANGED& pkt) { return MakeSendBuffer(pkt, PKT_S_STATCHANGED); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEVEL_UP& pkt) { return MakeSendBuffer(pkt, PKT_S_LEVEL_UP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_UPGRADESKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_UPGRADESKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_QUEST& pkt) { return MakeSendBuffer(pkt, PKT_S_QUEST); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LOAD_TARGET& pkt) { return MakeSendBuffer(pkt, PKT_S_LOAD_TARGET); }
	static SendBufferRef MakeSendBuffer(Protocol::S_QUICK_QUEST& pkt) { return MakeSendBuffer(pkt, PKT_S_QUICK_QUEST); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SET_DATA& pkt) { return MakeSendBuffer(pkt, PKT_S_SET_DATA); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
#endif

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};