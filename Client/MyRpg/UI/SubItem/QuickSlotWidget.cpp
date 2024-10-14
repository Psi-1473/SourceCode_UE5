// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/QuickSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Item.h"
#include "MyPlayer.h"
#include "Skill/Skill.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "DragWidget/SlotDrag.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlockImageDecorator.h"

//FReply UQuickSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	return FReply();
//}

bool UQuickSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	TObjectPtr<USlotDrag> DragOper = Cast<USlotDrag>(InOperation);

	if (DragOper != nullptr)
	{
		// 1. 드래그 된 슬롯이 유효한가 확인하고 유효하지 않으면 그냥 종료
		if (DragOper->Item != nullptr)
		{
			if (DragOper->Item->GetItemType() != Protocol::ITEM_CONSUMABLE)
			{
				return false;
			}
		}

		RemovePrevElement();
		SetElement(DragOper);
		Refresh();
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag Failed"));
		return false;
	}
}

void UQuickSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (CoolDown <= 0.0f)
	{
		return;
	}

	ProgressBar_Cooldown->SetPercent(CoolDown / MaxCoolDown);
	CoolDown -= InDeltaTime;
}

void UQuickSlotWidget::SetKey()
{
	FString KeyName = GetName();
	GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	GInstance->QuickSlots.Add(GetQuickType(), this);
	Text_Slot->SetText(FText::FromString(KeyName));
	Refresh();
}

void UQuickSlotWidget::Use(TObjectPtr<AMyPlayer> Player)
{
	if (Item != nullptr)
	{
		UseItem();
		return;
	}

	if (Skill != nullptr)
	{
		UseSkill(Player);
	}
}

void UQuickSlotWidget::SetItem(TObjectPtr<class UItem> NewItem)
{
	Empty();
	Item = NewItem;
	Item->SetQuickSlot(GetQuickType());

}

void UQuickSlotWidget::SetSkill(TObjectPtr<USkill> NewSkill)
{
	Empty();
	Skill = NewSkill;
	Skill->SetQuickSlot(GetQuickType());
}

void UQuickSlotWidget::Empty()
{
	Item = nullptr;
	Skill = nullptr;
	Refresh();
}

void UQuickSlotWidget::RemovePrevElement()
{
	if (Item != nullptr)
	{
		Item->SetQuickSlot(Protocol::QUICK_NONE);
	}
	
	if (Skill != nullptr)
	{
		Skill->SetQuickSlot(Protocol::QUICK_NONE);
	}
	
}
void UQuickSlotWidget::SetElement(TObjectPtr<USlotDrag> Drag)
{
	if (Drag->Item != nullptr)
	{
		// 이 아이템이 다른 퀵슬롯에 등록되어 있다면 제거
		GInstance->SetQuickSlotNull(Drag->Item->GetQuickSlotKey());

		// 이 퀵슬롯에 아이템 등록
		SetItem(Drag->Item);
		SendRegisterPakcet(Protocol::QUICK_ITEM);
	}
	if (Drag->Skill != nullptr)
	{
		// 기존 스킬이 등록되어 있던 퀵슬롯 정보는 비우기
		GInstance->SetQuickSlotNull(Drag->Skill->GetQuickSlotKey());

		SetSkill(Drag->Skill);
		SendRegisterPakcet(Protocol::QUICK_SKILL);
	}
}
void UQuickSlotWidget::UseItem()
{
	Item->UseItem(GInstance);
}
void UQuickSlotWidget::UseSkill(TObjectPtr<AMyPlayer> Player)
{
	Player->SendSkillPacket(Skill->GetSkillInfo()->SkillId);
}
void UQuickSlotWidget::SendRegisterPakcet(Protocol::EQuickElementType ElementType)
{
	Protocol::C_SET_QUICK Pkt;
	Pkt.set_type(ElementType);
	Pkt.set_slot(GetQuickType());
	if (ElementType == Protocol::QUICK_ITEM)
	{
		Pkt.set_elementid(Item->GetDbId());
	}
	else
	{
		Pkt.set_elementid(Skill->GetSkillInfo()->SkillId);
	}

	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
}
void UQuickSlotWidget::Refresh()
{
	if (Item != nullptr)
	{
		Img_Item->SetVisibility(ESlateVisibility::Visible);
		Img_Item->SetBrush(Item->GetItemImage()->Brush);
		Text_Count->SetText(FText::FromString(FString::FromInt(Item->GetCount())));
		return;
	}

	if (Skill != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SkillId : %d"), SkillId);
		Img_Item->SetVisibility(ESlateVisibility::Visible);
		Img_Item->SetBrush(Skill->GetSkillBrush());
		Text_Count->SetText(FText::FromString(TEXT("")));
		return;
	}
	Img_Item->SetVisibility(ESlateVisibility::Hidden);
	Text_Count->SetText(FText::FromString(TEXT("")));
}

Protocol::EQuickSlot UQuickSlotWidget::GetQuickType()
{
	FString Name = GetName();

	if (Name == TEXT("0"))
	{
		return Protocol::QUICK_0;
	}
	if (Name == TEXT("1"))
	{
		return Protocol::QUICK_1;
	}
	if (Name == TEXT("2"))
	{
		return Protocol::QUICK_2;
	}
	if (Name == TEXT("3"))
	{
		return Protocol::QUICK_3;
	}
	if (Name == TEXT("4"))
	{
		return Protocol::QUICK_4;
	}
	if (Name == TEXT("5"))
	{
		return Protocol::QUICK_5;
	}
	if (Name == TEXT("6"))
	{
		return Protocol::QUICK_6;
	}
	if (Name == TEXT("7"))
	{
		return Protocol::QUICK_7;
	}
	if (Name == TEXT("8"))
	{
		return Protocol::QUICK_8;
	}
	if (Name == TEXT("9"))
	{
		return Protocol::QUICK_9;
	}
	if (Name == TEXT("Q"))
	{
		return Protocol::QUICK_Q;
	}
	if (Name == TEXT("E"))
	{
		return Protocol::QUICK_E;
	}
	if (Name == TEXT("R"))
	{
		return Protocol::QUICK_R;
	}
	if (Name == TEXT("C"))
	{
		return Protocol::QUICK_C;
	}
	if (Name == TEXT("V"))
	{
		return Protocol::QUICK_V;
	}
	if (Name == TEXT("T"))
	{
		return Protocol::QUICK_T;
	}

	return Protocol::QUICK_NONE;
}