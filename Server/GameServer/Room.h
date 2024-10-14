#pragma once
#include "JobQueue.h"

struct UserBaseInfo;
struct CharacterBaseInfo;

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

/*
	Handle Player
*/

	void SetEnterPlayerInfo(PlayerRef player, uint64 characterId);
	void SetPlayerComponentInfo(PlayerRef player);

	bool HandleEnterPlayer(PlayerRef player, const UserBaseInfo base1, const CharacterBaseInfo base2);
	bool HandleLeavePlayer(PlayerRef player);
	void HandleMove(Protocol::C_MOVE pkt);
	void HandleAttack(Protocol::C_ATTACK pkt);
	void HandleUseSkill(PlayerRef player, Protocol::C_USESKILL pkt);

	void HandleDamaged(CreatureRef creature, uint64 attackerId, float damage);
	void HandleDie(CreatureRef creature);

	void HandleAddItem(Protocol::C_ADDITEM pkt, PlayerRef player);
	void HandleSetGold(Protocol::C_SETGOLD pkt, PlayerRef player);
	void HandleSwapItem(Protocol::C_ITEMSWAP pkt);
	void HandleQuickSlot(Protocol::C_SET_QUICK pkt, PlayerRef player);
	void HandleEquipItem(Protocol::C_EQUIP pkt, PlayerRef player);
	void HandleUnEquipItem(Protocol::C_UNEQUIP pkt, PlayerRef player);
	void HandleUseItem(Protocol::C_USEITEM pkt, PlayerRef player);
	void HandleRemoveItem(Protocol::C_REMOVEITEM pkt, PlayerRef player);

	void HandleUpgradeSkill(Protocol::C_UPGRADESKILL pkt, PlayerRef player);
	void HandleQuest(Protocol::C_QUEST pkt, PlayerRef player);
	void HandleRenewTarget(Protocol::C_RENEW_TARGET pkt, PlayerRef player);
	void HandleQuickQuest(Protocol::C_QUICK_QUEST pkt, PlayerRef player);

	void HitCheck(AttackParam params);
	void SetCreatureAttackable(CreatureRef object, bool value, int attackSequence);
	void SetCreatureIdle(CreatureRef object, Protocol::EState prevState);
	void SetCreatureIdleFromAttack(CreatureRef object, int attackSequence);

	void GiveQuestRewards(PlayerRef player, int32 questId);

/*
	Handle Monster
*/	void AddItem(Protocol::EItemType itemType, int32 itemId, int32 count, PlayerRef player);
	bool SpawnMonster(MonsterRef monster, bool isNew);
	void UpdateTick(shared_ptr<class BaseObject> object);

	const unordered_map<uint64, CreatureRef>& GetPlayers() { return _players; }
	RoomRef GetRoomRef();

public:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	void CreateSpawner();
	void Respawn(uint64 spawnerId);
private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);




private:
	unordered_map<uint64, shared_ptr<class MonsterSpawner>> _spawners;
	unordered_map<uint64, CreatureRef> _players;
	unordered_map<uint64, CreatureRef> _monsters;
};

extern RoomRef GRoom;