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
	/* ������Ʈ ��ü �ʱ�ȭ �Լ� */
	void Init(shared_ptr<class Player> player);

	/* �÷��̾ ó�� �������� ��, ������ ������ ������ Ŭ���̾�Ʈ���� �˷��ֱ� ���� �Լ� */
	void InitWithPlayerStart();

	/* ������ ���� �Լ� */
	void SetElement(Protocol::EQuickSlot slot, Protocol::EQuickElementType type, uint64 id);


private:
	unordered_map<Protocol::EQuickSlot, QuickElement> quickSlot;
	weak_ptr<class Player> owner;
};

