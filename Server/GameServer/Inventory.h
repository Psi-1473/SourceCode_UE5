#pragma once
#include "Equipment.h"

class Inventory
{
public:
	Inventory();

	/* ������Ʈ ��ü �ʱ�ȭ �Լ� */
	void Init(PlayerRef newOwner);

	
	/* Ư�� �������� �κ��丮�� �� �� ���� */
	void AddItemToInventory(ItemRef item, int slotIndex, bool isOnlyUneuqipResult, bool bSendToClient);
	/* ������ ���� �Լ� */
	void RemoveItem(uint64 dbId);

	/* ������ ���� ���� �Լ� */
	void EquipItem(uint64 dbId);
	void UnEquipItem(ItemRef unequipItem);

	/* ������ ��ġ ���� �Լ� */
	void SwapItem(const Protocol::C_ITEMSWAP& pkt);

	/* �κ��丮���� Ư�� �������� ��ġ�� ������ ä��ų� ��� �� ����� �Լ� */
	void FillSlot(ItemRef item);
	void EmptySlot(ItemRef item);

	int32 GetSpecificItemCount(Protocol::EItemType itemType, int32 templateId);
	/* ������ ������ ������ �÷��̾����� ���� ��Ű�� �Լ� */
	void ApplyItemStat();

	void SetGold(int32 value, bool bSendPacket = true);// { gold = value; }
	void AddGold(int32 value) { SetGold(gold + value); }
public:
	/* �κ��丮�� Ư�� ���� ����ִ���, ����ִٸ� �� ������ Index�� �Ѱ��ִ� �Լ� */
	int FindSlotIndex(Protocol::EItemType itemType);

	/* 
		1. ���� ���� ���� �� �ִ� �������ΰ�?
		2. �߰��Ϸ��� �����۰� ������ �������̸� �ִ� ������ �ƴ� �������� �����ϴ°�?
		
		* �� �� ���� �Ǵ��Ͽ� true or false�� ��ȯ�ϰ�
		* true��� 2���� �ش��ϴ� �������� DbId�� dbId ���ڿ� ����
	*/
	bool GetStackableDbId(Protocol::EItemType itemType, int32 templateId, uint64& dbId, int32 count = 1);


	shared_ptr<Item> GetItem(uint64 dbId) { return allItems[dbId]; }
	shared_ptr<Equipment> GetEquipItem(int32 equipSlotIndex) { return equippedItems[equipSlotIndex]; }
private:
	int GetEquippedSlotIndex(uint64 dbId);
	void UpdatePlayerStat(uint64 dbId, bool bIncrease);
	void PopSlotIndex(ItemRef item, int slotIndex);
private:
	unordered_map<uint64, shared_ptr<Item>> allItems;
	unordered_map<uint32, shared_ptr<Equipment>> equippedItems;

	// �� ������ priority_queue�� �̿��ؼ� ��������.!!
	set<int> emptySlot_Equipment;
	set<int> emptySlot_Consumable;
	int32 gold = 0;

private:
	const int MAX_ITEM_COUNT = 30;
	const int MAX_SLOT_COUNT = 36;
	weak_ptr<class Player> owner;

};

