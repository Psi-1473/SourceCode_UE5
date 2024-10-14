// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "DataManager.h"


void UConsumable::Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type)
{
	Super::Init(TemplateId, DataId, Cnt, SlotIdx, State, Type);
	auto GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	ItemData = GInstance->GetDataMgr()->GetConsumableData(TemplateId);
	ItemImage = GInstance->GetDataMgr()->GetConsumableImage(TemplateId);
}

void UConsumable::UseItem(TObjectPtr<UMyGameInstance> GInstance)
{
	Super::UseItem(GInstance);
	if (GetUsableItem(GInstance->GetPlayer().Get()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXXX"));
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Use Consumable "));

	Protocol::C_USEITEM Pkt;
	Pkt.set_itemid(DbId);

	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
}

const Protocol::ECharacterType UConsumable::GetClass()
{
	return Protocol::NONE;
}
