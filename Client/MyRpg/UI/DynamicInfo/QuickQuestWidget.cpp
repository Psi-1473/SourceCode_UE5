// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DynamicInfo/QuickQuestWidget.h"
#include "SubItem/QuickQuestElementWidget.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Components/ScrollBox.h"
#include "MyPlayer.h"

void UQuickQuestWidget::RegisterQuest(int32 QuestId, TObjectPtr<UMyGameInstance> Instance, bool bSendPacket)
{
	// 1. QuickQuestElement 생성
	if (QuickElementList.Contains(QuestId))
	{
		return;
	}
	if (GInstance == nullptr)
	{
		GInstance = Instance;
	}


	TObjectPtr<UQuickQuestElementWidget> Element = Cast<UQuickQuestElementWidget>(GInstance->GetUIMgr()->CreateSubItem(ElementClass));

	// 2. QuickQuestElement에 QuestId를 통해 Init
	Element->SetQuestInfo(QuestId, GInstance);

	// 3. QuickQuestList에 자식으로 등록
	QuickQuestList->AddChild(Element);

	QuickElementList.Add(QuestId, Element);

	// 패킷 보내기
	if(bSendPacket)
	{
		Protocol::C_QUICK_QUEST Pkt;
		Pkt.set_object_id(GInstance->UserDBId);
		Pkt.set_questid(QuestId);
		Pkt.set_isremove(false);

		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
		GInstance->SendPacket(SendBuffer);
	}
}

void UQuickQuestWidget::RemoveQuest(int32 QuestId)
{
	if (QuickElementList.Contains(QuestId))
	{
		QuickElementList[QuestId]->RemoveFromParent();
		QuickElementList.Remove(QuestId);
		Protocol::C_QUICK_QUEST Pkt;
		Pkt.set_object_id(GInstance->UserDBId);
		Pkt.set_questid(QuestId);
		Pkt.set_isremove(true);

		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
		GInstance->SendPacket(SendBuffer);
	}
}

void UQuickQuestWidget::RefreshElementUI(int32 QuestId)
{
	if (QuickElementList.Contains(QuestId))
	{
		QuickElementList[QuestId]->Refresh();
	}
}
