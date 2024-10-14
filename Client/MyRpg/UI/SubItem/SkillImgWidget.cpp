// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/SkillImgWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DragWidget/SlotDrag.h"
#include "Protocol.pb.h"
#include "Skill/Skill.h"
#include "MyGameInstance.h"
#include "DataManager.h"
#include "MyPlayer.h"
#include "Popup/SkillInfoWidget.h"
#include "UIManager.h"

void USkillImgWidget::Init(int32 Id, TObjectPtr<UMyGameInstance> Instance)
{
	GInstance = Instance;
	Protocol::ECharacterType Type = GInstance->GetPlayer()->GetObjectInfo()->objtype();
	Skill = GInstance->GetDataMgr()->GetSkill(Type, Id);
	
	Img_Skill->SetBrush(Skill->GetSkillBrush());
}

void USkillImgWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);
	if (GInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instance NUll"));
		return;
	}
	if (Skill == nullptr)
	{
		return;
	}
	auto Widget = Cast<USkillInfoWidget>(GInstance->GetUIMgr()->PopupUI("SkillInfo"));
	Widget->Init(Skill);
}

void USkillImgWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);
	if (GInstance == nullptr)
	{
		return;
	}

	GInstance->GetUIMgr()->CloseUI("SkillInfo");
}

FReply USkillImgWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" MOUSE DOWN "));

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		UE_LOG(LogTemp, Warning, TEXT("Drag Start"));
	}
	return Reply.NativeReply;
}

void USkillImgWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		TObjectPtr<USlotDrag> DragOper = NewObject<USlotDrag>();
		OutOperation = DragOper;
		DragOper->Skill = Skill;

		UE_LOG(LogTemp, Warning, TEXT("Dragging"));
		DragOper->Payload = this;
		DragOper->DefaultDragVisual = this;

	}
}
