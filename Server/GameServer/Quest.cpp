#include "pch.h"
#include "Quest.h"
#include "DataManager.h"
#include "DBStruct.h"
#include "Player.h"
#include "Inventory.h"

Quest::Quest(weak_ptr<Player> player, int32 id, int32 targetCount)
{
	questId = id;
	owner = player;
	// 1. Target 정보 불러와서 추가
	unordered_map<int, shared_ptr<FQuestTargetInfo>> targetInfo = GDataManager->GetQuestTargetData(id);
	cout << id << " Quest Construct " << endl;
	if (targetInfo.size() <= 0)
	{
		isCleared = true;
		Protocol::S_QUEST pkt;
		pkt.add_questid(questId);
		pkt.add_queststate(Protocol::QUEST_COMPLETABLE);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
		cout << "Completable!" << endl;
		return;
	}

	cout << "Progress Quest Target Num : " << targetInfo.size() << endl;
	maxTarget = targetInfo.size();
	for (int i = 0; i < targetInfo.size(); i++)
	{
		shared_ptr<FQuestTarget> info = make_shared<FQuestTarget>();

		info->targetMaxNumber = targetInfo[i]->targetNumber;

		// 나중에 아이템 퀘스트는 NowNumber를 아이템 체크해서 설정하자. 
		info->targetNowNumber = targetCount;
		info->targetIndex = i;
		if (targetInfo[i]->targetType == Protocol::TARGET_MONSTER)
		{
			huntTargets.insert({ targetInfo[i]->targetId, info});
		}
		if (targetInfo[i]->targetType == Protocol::TARGET_NPC)
		{
			npcTargets.insert({ targetInfo[i]->targetId, info });
		}
		if (targetInfo[i]->targetType == Protocol::TARGET_CONSUMABLE)
		{
			// 1. 플레이어 인벤토리 돌면서 해당 아이템 갯수 체크
			int32 targetCount = player.lock()->GetInventory()->GetSpecificItemCount(Protocol::ITEM_CONSUMABLE, targetInfo[i]->targetId);
			info->targetNowNumber = targetCount;
			itemTargets[Protocol::ITEM_CONSUMABLE][targetInfo[i]->targetId] = info;
			//itemTargets.insert({make_pair(Protocol::ITEM_CONSUMABLE, targetInfo[i]->targetId), info });
			
		}
		if (targetInfo[i]->targetType == Protocol::TARGET_MISC)
		{
			// 1. 플레이어 인벤토리 돌면서 해당 아이템 갯수 체크
			//player.lock()->GetInventory()->GetSpecificItemCount(targetInfo[i]->targetId);
		}

		targets.push_back(info);
	}
}


void Quest::SetTargetNumber(int32 targetIndex, int32 targetCount)
{
	targets[targetIndex]->targetNowNumber = targetCount;
	
	if (targets[targetIndex]->IsCleared())
		clearedTarget++;

	if(clearedTarget >= maxTarget)
		isCleared = true;

	if (isCleared)
	{
		Protocol::S_QUEST pkt;
		pkt.add_questid(questId);
		pkt.add_queststate(Protocol::QUEST_COMPLETABLE);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
		cout << "Completable!" << endl;
	}
}

void Quest::TargetNumberUp(Protocol::ETargetType type, int32 targetId, int32 count)
{
	if (isCleared)
	{
		if(type == Protocol::TARGET_MONSTER)
			return;

		if (type == Protocol::TARGET_NPC)
			return;
	}
		
	switch (type)
	{
		case Protocol::TARGET_MONSTER:
			HuntTargetNumberUp(targetId, count);
			break;
		case Protocol::TARGET_NPC:
			NpcTargetNumberUp(targetId, count);
			break;
		case Protocol::TARGET_CONSUMABLE:
			ItemTargetRefresh(Protocol::ITEM_CONSUMABLE, targetId);
			break;
		default:
			break;
	}

	
}

void Quest::HuntTargetNumberUp(int32 targetId, int32 count)
{
	if(huntTargets.contains(targetId) == false)
		return;

	if(huntTargets[targetId]->IsCleared())
		return;

	Protocol::S_LOAD_TARGET pkt;

	cout << "Hunt Quest Target Up : " << endl;
	huntTargets[targetId]->TargetNumberUp(count);
	pkt.add_questid(questId);
	pkt.add_targetcount(huntTargets[targetId]->targetNowNumber);
	pkt.add_targetindex(huntTargets[targetId]->targetIndex);
	if(huntTargets[targetId]->IsCleared())
		clearedTarget++;

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);

	CheckTargetCompleted();
	if (isCleared)
	{
		// 이 퀘스트 완료 가능!
		Protocol::S_QUEST pkt;
		pkt.add_questid(questId);
		pkt.add_queststate(Protocol::QUEST_COMPLETABLE);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
		cout << "Completable!" << endl;
	}

}

void Quest::NpcTargetNumberUp(int32 targetId, int32 count)
{
	if (npcTargets.contains(targetId) == false)
		return;

	if (npcTargets[targetId]->IsCleared())
		return;

	Protocol::S_LOAD_TARGET pkt;
	cout << "Npc Quest Target Up : " << endl;
	npcTargets[targetId]->TargetNumberUp(count);

	pkt.add_questid(questId);
	pkt.add_targetcount(npcTargets[targetId]->targetNowNumber);
	pkt.add_targetindex(npcTargets[targetId]->targetIndex);
	if (npcTargets[targetId]->IsCleared())
		clearedTarget++;

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);

	CheckTargetCompleted();
	if (isCleared)
	{
		// 이 퀘스트 완료 가능!
		Protocol::S_QUEST pkt;
		pkt.add_questid(questId);
		pkt.add_queststate(Protocol::QUEST_COMPLETABLE);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
		cout << "Completable!" << endl;
	}
}

void Quest::ItemTargetRefresh(Protocol::EItemType itemType, int32 itemId)
{
	if(itemTargets.contains(itemType) == false)
		return;

	if (itemTargets[itemType].contains(itemId) == false)
		return;

	bool isTargetCleared = itemTargets[itemType][itemId]->IsCleared();
	itemTargets[itemType][itemId]->targetNowNumber = owner.lock()->GetInventory()->GetSpecificItemCount(itemType, itemId);


	Protocol::S_LOAD_TARGET pkt;
	pkt.add_questid(questId);
	pkt.add_targetcount(itemTargets[itemType][itemId]->targetNowNumber);
	pkt.add_targetindex(itemTargets[itemType][itemId]->targetIndex);
	cout << "Item Target Number Update : Now Num = " << itemTargets[itemType][itemId]->targetNowNumber << endl;

	bool canToCant = false;
	if (isTargetCleared == true && itemTargets[itemType][itemId]->IsCleared() == false)
	{
		// 1. 기존에 완료 가능이었다가 (이전엔 퀘스트 조건 충족) 아이템 수가 줄어들어 완료 불가능 상태가 되어버린 상황
		clearedTarget--;
		canToCant = true;
	}
	if (isTargetCleared == false && itemTargets[itemType][itemId]->IsCleared())
	{
		clearedTarget++;
	}

	//CheckTargetCompleted();
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);

	CheckTargetCompleted();
	if (isCleared)
	{
		// 이 퀘스트 완료 가능!
		Protocol::S_QUEST pkt;
		pkt.add_questid(questId);
		pkt.add_queststate(Protocol::QUEST_COMPLETABLE);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		owner.lock()->session.lock()->Send(sendBuffer);
		cout << "Completable!" << endl;
	}
	else
	{
		if (canToCant)
		{
			cout << " Possible -> Impossible" << endl;
		}
	}

}

void Quest::CheckTargetCompleted()
{
	if(clearedTarget >= maxTarget)
	{
		cout << "Quest - You can clear" << endl;
		isCleared = true;
	}
	else
	{
		cout << "Quest - You can't clear" << endl;
		isCleared = false;
	}
}
