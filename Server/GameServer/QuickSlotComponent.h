#pragma once
#include "Protocol.pb.h"

struct QuickElement
{
	uint64 id;
	Protocol::EQuickElementType type;

	QuickElement() {}
	QuickElement(uint64 _id, Protocol::EQuickElementType _type) 
	: id(_id), type(_type) {}
};

class QuickSlotComponent
{
public:
	/* 컴포넌트 자체 초기화 함수 */
	void Init(shared_ptr<class Player> player);

	/* 플레이어가 처음 입장했을 때, 기존의 퀵슬롯 정보를 클라이언트에게 알려주기 위한 함수 */
	void InitWithPlayerStart();

	/* 퀵슬롯 세팅 함수 */
	void SetElement(Protocol::EQuickSlot slot, Protocol::EQuickElementType type, uint64 id);


private:
	unordered_map<Protocol::EQuickSlot, QuickElement> quickSlot;
	weak_ptr<class Player> owner;
};

