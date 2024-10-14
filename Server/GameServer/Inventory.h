#pragma once
#include "Equipment.h"

class Inventory
{
public:
	Inventory();

	/* 컴포넌트 자체 초기화 함수 */
	void Init(PlayerRef newOwner);

	
	/* 특정 아이템이 인벤토리로 들어갈 때 실행 */
	void AddItemToInventory(ItemRef item, int slotIndex, bool isOnlyUneuqipResult, bool bSendToClient);
	/* 아이템 제거 함수 */
	void RemoveItem(uint64 dbId);

	/* 아이템 장착 관련 함수 */
	void EquipItem(uint64 dbId);
	void UnEquipItem(ItemRef unequipItem);

	/* 아이템 위치 변경 함수 */
	void SwapItem(const Protocol::C_ITEMSWAP& pkt);

	/* 인벤토리에서 특정 아이템이 위치한 슬롯을 채우거나 비울 때 사용할 함수 */
	void FillSlot(ItemRef item);
	void EmptySlot(ItemRef item);

	int32 GetSpecificItemCount(Protocol::EItemType itemType, int32 templateId);
	/* 장착한 아이템 정보를 플레이어한테 적용 시키는 함수 */
	void ApplyItemStat();

	void SetGold(int32 value, bool bSendPacket = true);// { gold = value; }
	void AddGold(int32 value) { SetGold(gold + value); }
public:
	/* 인벤토리의 특정 탭이 비어있는지, 비어있다면 빈 슬롯의 Index를 넘겨주는 함수 */
	int FindSlotIndex(Protocol::EItemType itemType);

	/* 
		1. 여러 개를 가질 수 있는 아이템인가?
		2. 추가하려는 아이템과 동일한 아이템이며 최대 갯수가 아닌 아이템이 존재하는가?
		
		* 위 두 개를 판단하여 true or false를 반환하고
		* true라면 2번에 해당하는 아이템의 DbId를 dbId 인자에 전달
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

	// 빈 슬롯은 priority_queue를 이용해서 관리하자.!!
	set<int> emptySlot_Equipment;
	set<int> emptySlot_Consumable;
	int32 gold = 0;

private:
	const int MAX_ITEM_COUNT = 30;
	const int MAX_SLOT_COUNT = 36;
	weak_ptr<class Player> owner;

};

