#include "pch.h"
#include "QuestComponent.h"
#include "Player.h"
#include "Quest.h"

//void QuestComponent::AddNewAvailableQuest(int32 questId)
//{
//	AddToAvilable(questId);
//}
//
//void QuestComponent::AcceptNewQuest(int32 questId, int32 targetCount)
//{
//	AddToInProgress(questId, targetCount);
//	DeleteFromAvailable(questId);
//}
//
//void QuestComponent::AbandonQuest(int32 questId)
//{
//	DeleteFromInProgress(questId);
//	AddToAvilable(questId);
//}
//
//void QuestComponent::CompleteQuest(int32 questId)
//{
//	DeleteFromInProgress(questId);
//}

void QuestComponent::Init(weak_ptr<class Player> player)
{
	owner = player;
}

void QuestComponent::AddToAvailable(int32 questId)
{
	availableQuests.insert(questId);

	// ��Ŷ ������
}

void QuestComponent::AddToInProgress(int32 questId, int32 targetCount)
{
	shared_ptr<Quest> newQuest = make_shared<Quest>(owner, questId, targetCount);
	inProgressQuests[questId] = newQuest;

	// ��Ŷ ������
}

void QuestComponent::DeleteFromAvailable(int32 questId)
{
	availableQuests.erase(questId);

	// ��Ŷ ������
}

void QuestComponent::DeleteFromInProgress(int32 questId)
{
	inProgressQuests.erase(questId);

	// ��Ŷ ������
}

void QuestComponent::TargetUpSpecificQuest(int32 questId, int32 targetId, Protocol::ETargetType type, int32 count)
{
	inProgressQuests[questId]->TargetNumberUp(type, targetId, count);
}

void QuestComponent::TargetUp(int32 targetId, Protocol::ETargetType type, int32 count)
{
	for (const auto& quest : inProgressQuests)
	{
		quest.second->TargetNumberUp(type, targetId, count);
	}

	
}
