#include "pch.h"
#include "QuickSlotComponent.h"
#include "DataManager.h"
#include "Player.h"

void QuickSlotComponent::Init(shared_ptr<class Player> player)
{
	owner = player;
}

void QuickSlotComponent::InitWithPlayerStart()
{
	Protocol::S_LOAD_QUICK pkt;
	for (const auto& value : quickSlot)
	{
		pkt.add_slot(value.first);
		pkt.add_type(value.second.type);
		pkt.add_elementid(value.second.id);
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);
}

void QuickSlotComponent::SetElement(Protocol::EQuickSlot slot, Protocol::EQuickElementType type, uint64 id)
{
	// 1. 이미 담겨있던 퀵슬롯 요소가 있다면 DB에서 퀵슬롯 해제 처리
	if(quickSlot.count(slot) > 0)
	{
		QuickElement prevElement = quickSlot[slot];
		GDBManager->DoAsync(&DBManager::SetQuickSlot, Protocol::QUICK_NONE, prevElement.id, prevElement.type, owner.lock());
	}

	// 2. 퀵슬롯에 새로운 요소 담기
	quickSlot[slot] = QuickElement(id, type);
	cout << "Set Quick Slot[" << slot << "] ----> (" << type << ") " << id << endl;
	

}
