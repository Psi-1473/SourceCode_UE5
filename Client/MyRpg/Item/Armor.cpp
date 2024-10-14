// Fill out your copyright notice in the Description page of Project Settings.


#include "Armor.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "DataManager.h"


void UArmor::Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type)
{
	Super::Init(TemplateId, DataId, Cnt, SlotIdx, State, Type);
	auto GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	ItemData = GInstance->GetDataMgr()->GetArmorData(TemplateId);
	ItemImage = GInstance->GetDataMgr()->GetArmorImage(TemplateId);
}

void UArmor::UseItem(TObjectPtr<UMyGameInstance> GInstance)
{
	Super::UseItem(GInstance);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Armor Equip "));
	if (GetUsableItem(GInstance->GetPlayer().Get()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXXX"));
		return;
	}
	// 장착 패킷 보내기
	Protocol::C_EQUIP Pkt;
	Pkt.set_itemid(DbId);

	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
}

void UArmor::UnEquip(TObjectPtr<class UMyGameInstance> GInstance)
{
	Protocol::C_UNEQUIP Pkt;
	Pkt.set_itemid(DbId);

	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
}

const Protocol::ECharacterType UArmor::GetClass()
{
	return (Protocol::ECharacterType)(static_cast<FArmorData*>(ItemData)->Class);
}
