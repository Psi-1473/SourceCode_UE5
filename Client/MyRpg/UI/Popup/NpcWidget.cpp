// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/NpcWidget.h"
#include "Npc.h"
#include "UObject/ConstructorHelpers.h"
#include "Data/DataStruct.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "SubItem/InteractListWidget.h"
#include "Popup/NpcQuestInfo.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "DataManager.h"
#include "MyPlayer.h"
#include "ActorComponents/QuestComponent.h"

UNpcWidget::UNpcWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InterClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/SubItem/WBP_InteractList.WBP_InteractList_C'"));

	if (InterClass.Succeeded())
	{
		InteractListClass = InterClass.Class;
	}

	//DialoguePtr = new FQuestDialogue();

}

void UNpcWidget::Init(TObjectPtr<ANpc> NewNpc, TObjectPtr<UMyGameInstance> Instance)
{
	GInstance = Instance;
	OwnerNpc = NewNpc;

	Btn_LineNext->OnClicked.AddDynamic(this, &UNpcWidget::ShowNextDialogue);
	Btn_LineNext->SetVisibility(ESlateVisibility::Hidden);
	
	SetBaseInfo();

	// -===== ��ȣ�ۿ� ��� �����ϱ� =====-
	
	// 1. ���� (���� Npc���)
	if (OwnerNpc->GetNpcData()->Items.Len() > 0)
	{
		SetInteractInfo(InteractUIType::SHOP);
	}
	// 2. ���� ���� (����Ʈ, ������ ��)
	for (int i = 1; i < 3; i++)
	{
		SetInteractInfo((InteractUIType(i)));
	}


}

void UNpcWidget::SetDialogueMode(int32 QuestId, int32 Type)
{
	// 1. ù���� ��� �����ֱ�
	//GInstance->GetDataMgr()->
	InteractList->SetVisibility(ESlateVisibility::Hidden);
	Btn_LineNext->SetVisibility(ESlateVisibility::Visible);

	SetDialogueInfo(QuestId, Type);
	ShowNextDialogue();

}


void UNpcWidget::SetGeneralMode()
{
	InteractList->SetVisibility(ESlateVisibility::Visible);
	Btn_LineNext->SetVisibility(ESlateVisibility::Hidden);
}


void UNpcWidget::ShowNextDialogue()
{
	if (Dialogue == nullptr)
	{
		return;
	}

	DialogueIndex++;
	if (Dialogue->Dialogue.Num() <= DialogueIndex)
	{
		// ����Ʈ ���� ������ ����
		Dialogue = nullptr;
		DialogueIndex = -1;

		if (DialogueType == 2)
		{
			OwnerNpc->RemoveTargetQuest();
			// ��Ŷ ������

			{
				Protocol::C_RENEW_TARGET TargetPkt;

				TargetPkt.set_targetcount(1);
				TargetPkt.set_targettype(Protocol::TARGET_NPC);
				TargetPkt.set_targetid(OwnerNpc->GetId());
				TargetPkt.set_questid(DialogueQuestId);

				SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(TargetPkt);
				GInstance->SendPacket(SendBuffer);
			}

			GInstance->GetUIMgr()->CloseUI(TEXT("Npc"));
			return;
		}
		auto Widget = Cast<UNpcQuestInfo>(GInstance->GetUIMgr()->PopupUI(TEXT("NpcQuestInfo")));
		bool bCompletable = false;
		if(DialogueType == 1)
		{
			bCompletable = true;
		}
		Widget->Init(OwnerNpc, GInstance, DialogueQuestId, bCompletable);
		
	}
	else
	{
		Text_Line->SetText(FText::FromString(Dialogue->Dialogue[DialogueIndex]->Dialogue));
	}
}



/*
	Pirvate
*/

void UNpcWidget::SetBaseInfo()
{
	// 0. Npc ���� ��������
	FNpcData* Data = OwnerNpc.Get()->GetNpcData();

	// 1. �»���� NPC �⺻ ���� �����ϱ� 
	Text_Name->SetText(FText::FromString(Data->NpcName));
	Text_Title->SetText(FText::FromString(Data->Title));

	// 2. ��� �����ϱ�
	Text_Line->SetText(FText::FromString(Data->DefaultGreeting));

}

void UNpcWidget::SetInteractInfo(InteractUIType InteractType)
{
	TObjectPtr<UInteractListWidget> Widget = Cast<UInteractListWidget>(GInstance.Get()->GetUIMgr()->CreateSubItem(InteractListClass));
	InteractList->AddChild(Widget);
	Widget->SetPadding(FMargin(30.f, 0.f, 0.f, 0.f));
	Widget->Init(InteractType, OwnerNpc, GInstance);
}

void UNpcWidget::SetDialogueInfo(int32 QuestId, int32 Type)
{
	Dialogue = GInstance->GetDataMgr()->GetQuestDialogue(QuestId, OwnerNpc->GetId(), Type);
	FString Key = FString::Printf(TEXT("%d_%d"), OwnerNpc->GetId(), QuestId);
	DialogueQuestId = QuestId;
	DialogueType = Type;
}
