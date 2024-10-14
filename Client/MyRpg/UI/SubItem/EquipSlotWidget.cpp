// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubItem/EquipSlotWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Item.h"
#include "Item/Armor.h"
#include "Item/Weapon.h"
#include "Components/RichTextBlockImageDecorator.h"

void UEquipSlotWidget::Init(TObjectPtr<class UMyGameInstance> Instance, TObjectPtr<class UItem> NewItem)
{
	GInstance = Instance;
	Item = NewItem;

	if(Item != nullptr)
	SetInfo();
}

FReply UEquipSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(MyGeometry, MouseEvent);

	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		
		if (Item != nullptr)
		{
			auto Weapon = Cast<UWeapon>(Item);
			if (Weapon != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Weapon UNEQUIP "));
				Weapon->UnEquip(GInstance);
			}

			auto Armor = Cast<UArmor>(Item);
			if (Armor != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Armor UNEQUIP "));
				Armor->UnEquip(GInstance);
			}
		}
	}


	return Reply.NativeReply;
}

void UEquipSlotWidget::SetInfo()
{
	if (Item == nullptr)
	{
		FSlateBrush MyBrush;
		MyBrush.SetResourceObject(BaseImage);
		MyBrush.ImageSize = FVector2D(128, 128);
		MyBrush.DrawAs = ESlateBrushDrawType::Image;
		Img_Item->SetBrush(MyBrush);
		
	}
	else
	{
		Img_Item->SetBrush(Item->GetItemImage()->Brush);
	}
}
