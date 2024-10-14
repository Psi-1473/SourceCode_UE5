#pragma once

class Quest;

class QuestComponent
{
public:
	const unordered_map<int32, shared_ptr<Quest>>& GetInProgressQuests() { return inProgressQuests; }
	shared_ptr<Quest> GetInProgressQuest(int32 questId) { return inProgressQuests[questId]; }

public:
	void Init(weak_ptr<class Player> player);
	void AddToAvailable(int32 questId);
	void AddToInProgress(int32 questId, int32 targetCount = 0);
	void DeleteFromAvailable(int32 questId);
	void DeleteFromInProgress(int32 questId);

	void TargetUpSpecificQuest(int32 questId, int32 targetId, Protocol::ETargetType type, int32 count = 1);
	void TargetUp(int32 targetId, Protocol::ETargetType type, int32 count = 1);
private:
	set<int32> availableQuests;
	unordered_map<int32, shared_ptr<Quest>> inProgressQuests;
	weak_ptr<class Player> owner;




	// µ•¿Ã≈Õ
	array<int, 12> t;\
	
};

