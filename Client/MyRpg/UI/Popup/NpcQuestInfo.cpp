// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/NpcQuestInfo.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "UIManager.h"
#include "Npc.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Data/DataStruct.h"
#include "SubItem/QuestRewardWidget.h"

void UNpcQuestInfo::Init(TWeakObjectPtr<ANpc> Npc, TObjectPtr<UMyGameInstance> Instance, int32 NewQuestId, bool IsCompletable)
{
	GInstance = Instance;
	QuestId = NewQuestId;
	bCompletable = IsCompletable;

	Btn_Exit->OnClicked.AddDynamic(this, &UNpcQuestInfo::CloseThisUI);
	Btn_Accept->OnClicked.AddDynamic(this, &UNpcQuestInfo::AcceptQuest);

	SetBaseInfo();
	SetRewardInfo();
}

void UNpcQuestInfo::CloseThisUI()
{
	GInstance->GetUIMgr()->CloseUI(TEXT("NpcQuestInfo"));
	GInstance->GetUIMgr()->CloseUI(TEXT("Npc"));
}

void UNpcQuestInfo::AcceptQuest()
{
	// 1. 플레이어 퀘스트 수주 패킷 보내기
	Protocol::C_QUEST Pkt;

	Pkt.add_questid(QuestId);
	if (bCompletable)
	{
		Pkt.add_queststate(Protocol::QUEST_COMPLETED);
	}
	else {
		Pkt.add_queststate(Protocol::QUEST_ACCEPT);
	}
	
	
	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
	// 2. 이 UI 닫기

	CloseThisUI();
}


/*
	===================== Priavate ===================
*/
void UNpcQuestInfo::SetBaseInfo()
{
	FQuestData* QuestData = GInstance->GetDataMgr()->GetQuestData(QuestId);

	Text_QuestName->SetText(FText::FromString(QuestData->QuestName));
	Text_QuestDesc->SetText(FText::FromString(QuestData->QuestDesc));

	if (bCompletable)
	{
		Text_Accept->SetText(FText::FromString(TEXT("완료")));
	}
}

void UNpcQuestInfo::SetRewardInfo()
{
	FQuestData* QuestData = GInstance->GetDataMgr()->GetQuestData(QuestId);
	TSharedPtr<FQuestReward> Rewards = GInstance->GetDataMgr()->GetQuestRewards(QuestId);

	int32 Exp = QuestData->Exp;
	int32 Gold = QuestData->Gold;

	if (Exp > 0)
	{
		CreateRewardWidget()->SetExp(Exp, GInstance);
	}

	if (Gold > 0)
	{
		CreateRewardWidget()->SetGold(Gold, GInstance);
	}

	if (Rewards != nullptr)
	{
		for (int i = 0; i < Rewards->Rewards.Num(); i++)
		{
			Protocol::EItemType ItemType = Rewards->Rewards[i]->ItemType;
			int32 ItemId = Rewards->Rewards[i]->ItemId;
			int32 ItemCount = Rewards->Rewards[i]->ItemCount;

			CreateRewardWidget()->SetItem(ItemType, ItemId, ItemCount, GInstance);
		}
	}


	
}

TObjectPtr<UQuestRewardWidget> UNpcQuestInfo::CreateRewardWidget()
{
	TObjectPtr<UQuestRewardWidget> Widget = Cast<UQuestRewardWidget>(GInstance.Get()->GetUIMgr()->CreateSubItem(RewardClass));
	HorizontalBox_Rewards->AddChild(Widget);
	
	return Widget;
}
