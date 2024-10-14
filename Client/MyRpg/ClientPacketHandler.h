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
bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_CHECKNAME(PacketSessionRef& session, Protocol::S_CHECKNAME& pkt);
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_LOAD_SKILL(PacketSessionRef& session, Protocol::S_LOAD_SKILL& pkt);
bool Handle_S_LOAD_QUICK(PacketSessionRef& session, Protocol::S_LOAD_QUICK& pkt);
bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_ATTACK(PacketSessionRef& session, Protocol::S_ATTACK& pkt);
bool Handle_S_USESKILL(PacketSessionRef& session, Protocol::S_USESKILL& pkt);
bool Handle_S_DAMAGED(PacketSessionRef& session, Protocol::S_DAMAGED& pkt);
bool Handle_S_DIE(PacketSessionRef& session, Protocol::S_DIE& pkt);
bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);
bool Handle_S_SETGOLD(PacketSessionRef& session, Protocol::S_SETGOLD& pkt);
bool Handle_S_ADDITEM(PacketSessionRef& session, Protocol::S_ADDITEM& pkt);
bool Handle_S_EQUIP(PacketSessionRef& session, Protocol::S_EQUIP& pkt);
bool Handle_S_SETITEMCOUNT(PacketSessionRef& session, Protocol::S_SETITEMCOUNT& pkt);
bool Handle_S_REMOVEITEM(PacketSessionRef& session, Protocol::S_REMOVEITEM& pkt);
bool Handle_S_STATCHANGED(PacketSessionRef& session, Protocol::S_STATCHANGED& pkt);
bool Handle_S_LEVEL_UP(PacketSessionRef& session, Protocol::S_LEVEL_UP& pkt);
bool Handle_S_UPGRADESKILL(PacketSessionRef& session, Protocol::S_UPGRADESKILL& pkt);
bool Handle_S_QUEST(PacketSessionRef& session, Protocol::S_QUEST& pkt);
bool Handle_S_LOAD_TARGET(PacketSessionRef& session, Protocol::S_LOAD_TARGET& pkt);
bool Handle_S_QUICK_QUEST(PacketSessionRef& session, Protocol::S_QUICK_QUEST& pkt);
bool Handle_S_SET_DATA(PacketSessionRef& session, Protocol::S_SET_DATA& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_S_CHECKNAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHECKNAME>(Handle_S_CHECKNAME, session, buffer, len); };
		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_LOAD_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOAD_SKILL>(Handle_S_LOAD_SKILL, session, buffer, len); };
		GPacketHandler[PKT_S_LOAD_QUICK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOAD_QUICK>(Handle_S_LOAD_QUICK, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_ATTACK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ATTACK>(Handle_S_ATTACK, session, buffer, len); };
		GPacketHandler[PKT_S_USESKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_USESKILL>(Handle_S_USESKILL, session, buffer, len); };
		GPacketHandler[PKT_S_DAMAGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DAMAGED>(Handle_S_DAMAGED, session, buffer, len); };
		GPacketHandler[PKT_S_DIE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DIE>(Handle_S_DIE, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_SETGOLD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SETGOLD>(Handle_S_SETGOLD, session, buffer, len); };
		GPacketHandler[PKT_S_ADDITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ADDITEM>(Handle_S_ADDITEM, session, buffer, len); };
		GPacketHandler[PKT_S_EQUIP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EQUIP>(Handle_S_EQUIP, session, buffer, len); };
		GPacketHandler[PKT_S_SETITEMCOUNT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SETITEMCOUNT>(Handle_S_SETITEMCOUNT, session, buffer, len); };
		GPacketHandler[PKT_S_REMOVEITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_REMOVEITEM>(Handle_S_REMOVEITEM, session, buffer, len); };
		GPacketHandler[PKT_S_STATCHANGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_STATCHANGED>(Handle_S_STATCHANGED, session, buffer, len); };
		GPacketHandler[PKT_S_LEVEL_UP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEVEL_UP>(Handle_S_LEVEL_UP, session, buffer, len); };
		GPacketHandler[PKT_S_UPGRADESKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_UPGRADESKILL>(Handle_S_UPGRADESKILL, session, buffer, len); };
		GPacketHandler[PKT_S_QUEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_QUEST>(Handle_S_QUEST, session, buffer, len); };
		GPacketHandler[PKT_S_LOAD_TARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOAD_TARGET>(Handle_S_LOAD_TARGET, session, buffer, len); };
		GPacketHandler[PKT_S_QUICK_QUEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_QUICK_QUEST>(Handle_S_QUICK_QUEST, session, buffer, len); };
		GPacketHandler[PKT_S_SET_DATA] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SET_DATA>(Handle_S_SET_DATA, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHECKNAME& pkt) { return MakeSendBuffer(pkt, PKT_C_CHECKNAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LOAD_INFO& pkt) { return MakeSendBuffer(pkt, PKT_C_LOAD_INFO); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CREATECHARACTER& pkt) { return MakeSendBuffer(pkt, PKT_C_CREATECHARACTER); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ATTACK& pkt) { return MakeSendBuffer(pkt, PKT_C_ATTACK); }
	static SendBufferRef MakeSendBuffer(Protocol::C_USESKILL& pkt) { return MakeSendBuffer(pkt, PKT_C_USESKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SETGOLD& pkt) { return MakeSendBuffer(pkt, PKT_C_SETGOLD); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ADDITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_ADDITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ITEMSWAP& pkt) { return MakeSendBuffer(pkt, PKT_C_ITEMSWAP); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SET_QUICK& pkt) { return MakeSendBuffer(pkt, PKT_C_SET_QUICK); }
	static SendBufferRef MakeSendBuffer(Protocol::C_EQUIP& pkt) { return MakeSendBuffer(pkt, PKT_C_EQUIP); }
	static SendBufferRef MakeSendBuffer(Protocol::C_UNEQUIP& pkt) { return MakeSendBuffer(pkt, PKT_C_UNEQUIP); }
	static SendBufferRef MakeSendBuffer(Protocol::C_USEITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_USEITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_REMOVEITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_REMOVEITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_UPGRADESKILL& pkt) { return MakeSendBuffer(pkt, PKT_C_UPGRADESKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_QUEST& pkt) { return MakeSendBuffer(pkt, PKT_C_QUEST); }
	static SendBufferRef MakeSendBuffer(Protocol::C_RENEW_TARGET& pkt) { return MakeSendBuffer(pkt, PKT_C_RENEW_TARGET); }
	static SendBufferRef MakeSendBuffer(Protocol::C_QUICK_QUEST& pkt) { return MakeSendBuffer(pkt, PKT_C_QUICK_QUEST); }

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