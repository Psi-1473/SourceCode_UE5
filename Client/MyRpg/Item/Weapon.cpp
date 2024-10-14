// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyGameInstance.h"
#include "MyPlayer.h"
#include "DataManager.h"

void UWeapon::Init(int32 TemplateId, uint64 DataId, int32 Cnt, int32 SlotIdx, Protocol::ESlotState State, Protocol::EItemType Type)
{
	Super::Init(TemplateId, DataId, Cnt, SlotIdx, State, Type);
	auto GInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance());
	ItemData = GInstance->GetDataMgr()->GetWeaponData(TemplateId);
	ItemImage = GInstance->GetDataMgr()->GetWeaponImage(TemplateId);
}

void UWeapon::UseItem(TObjectPtr<UMyGameInstance> GInstance)
{
	Super::UseItem(GInstance);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT(" Weapon Equip "));
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

void UWeapon::UnEquip(TObjectPtr<class UMyGameInstance> GInstance)
{
	Protocol::C_UNEQUIP Pkt;
	Pkt.set_itemid(DbId);

	SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
	GInstance->SendPacket(SendBuffer);
}

const Protocol::ECharacterType UWeapon::GetClass()
{
	return (Protocol::ECharacterType)(static_cast<FWeaponData*>(ItemData)->Class);
}
