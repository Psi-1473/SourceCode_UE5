#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#pragma comment(lib, "MySql\\Debug\\lib64\\debug\\vs14\\mysqlcppconn.lib")
#pragma comment(lib, "MySql\\Release\\lib64\\libmysql.lib")
//#pragma comment(lib, "MySql\\lib64\\vs14\\mysqlcppconnx.lib")

#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#pragma comment(lib, "MySql\\Release\\lib64\\mysqlcppconn.lib")
#pragma comment(lib, "MySql\\Release\\lib64\\libmysql.lib")
//#pragma comment(lib, "MySql\\lib64\\vs14\\mysqlcppconnx.lib")
#endif

#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "ServerPacketHandler.h"
#include "Utils.h"
#include "Geometry.h"
#include "GameSession.h"

USING_SHARED_PTR(GameSession);
USING_SHARED_PTR(BaseObject);
USING_SHARED_PTR(Creature);
USING_SHARED_PTR(Player);
USING_SHARED_PTR(Monster);
USING_SHARED_PTR(Room);
USING_SHARED_PTR(Item);
USING_SHARED_PTR(Equipment);
USING_SHARED_PTR(Weapon);
USING_SHARED_PTR(Armor);
USING_SHARED_PTR(Consumable);
USING_SHARED_PTR(Inventory);
USING_SHARED_PTR(SkillComponent);
USING_SHARED_PTR(QuickSlotComponent);


#define SEND_PACKET(pkt)													\
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);	\
	session->Send(sendBuffer);

#define PTR_CAST(CLASS, object)									\
	static_pointer_cast<CLASS>(object)
	
#define CAST(CLASS, object)									\
	static_cast<CLASS>(object);

#define THIS shared_from_this()

#define NESTED_UNORDERED_MAP(key1, key2, value)							\
	unordered_map<key1, unordered_map<key2, value>>





	/*
		Param Struct
	*/

struct AttackParam
{
	shared_ptr<BaseObject> attacker;
	Vec3 attackerRotation;
	Vec3 offset;
	Vec3 range;
	string hitType;
	int32 power;

	AttackParam() {};
	AttackParam(shared_ptr<BaseObject> _attacker, Vec3 _rot, Vec3 _offset, Vec3 _range, string _hitType, int32 _power)
	: attacker(_attacker), attackerRotation(_rot), offset(_offset), range(_range), hitType(_hitType), power(_power) {}
};