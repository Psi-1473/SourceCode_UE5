#pragma once


struct FQuestTarget
{
	int32 targetIndex;
	int32 targetNowNumber;
	int32 targetMaxNumber;

	void TargetNumberUp(int32 count = 1) { 
	targetNowNumber = targetNowNumber + count;
	if(targetNowNumber > targetMaxNumber) 
		targetNowNumber = targetMaxNumber; 
	}
	bool IsCleared() { return (targetNowNumber >= targetMaxNumber); }
};
class Quest
{
public:
	Quest() = delete;
	Quest(weak_ptr<class Player> player, int32 id, int32 targetCount = 0);

	void SetTargetNumber(int32 targetIndex, int32 targetCount);
	void TargetNumberUp(Protocol::ETargetType type, int32 targetId, int32 count = 1);

public:
	int32 GetQuestId() { return questId; }
	const vector<shared_ptr<FQuestTarget>>& GetAllTargetInfo() { return targets; }
private:
	void HuntTargetNumberUp(int32 targetId, int32 count);
	void NpcTargetNumberUp(int32 targetId, int32 count);
	void ItemTargetRefresh(Protocol::EItemType itemType, int32 itemId);
	void CheckTargetCompleted();

private:
	int32 questId;
	// ��� Ÿ�� ���� : key = targetId;
	vector<shared_ptr<FQuestTarget>> targets;
	unordered_map<int32, shared_ptr<FQuestTarget>> huntTargets;
	// npc Ÿ�� ����
	unordered_map<int32, shared_ptr<FQuestTarget>> npcTargets;
	// item Ÿ�� ����
	NESTED_UNORDERED_MAP(Protocol::EItemType, int32, shared_ptr<FQuestTarget>) itemTargets;

	int32 maxTarget = 0;
	int32 clearedTarget = 0;
	weak_ptr<class Player> owner;
	bool isCleared = false;
};

