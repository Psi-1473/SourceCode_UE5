#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "MyRpg.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "Popup/SelectCharacterWidget.h"
#include "Popup/CreateCharacterWidget.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->UserDBId = pkt.userdbid();
		TObjectPtr<USelectCharacterWidget> Widget = Cast<USelectCharacterWidget>(GameInstance->GetUIMgr()->GetPopupUI(TEXT("CharacterSelecet")));
		if (Widget == nullptr)
		{
			Widget = Cast<USelectCharacterWidget>(GameInstance->GetUIMgr()->PopupUI(TEXT("CharacterSelecet")));
			Widget->Init(GameInstance);
		}

		Widget->SetSlot(pkt);
		GameInstance->GetUIMgr()->CloseUI(TEXT("CreateCharacter"));
	}

	return true;
}

bool Handle_S_CHECKNAME(PacketSessionRef& session, Protocol::S_CHECKNAME& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		TObjectPtr<UCreateCharacterWidget> Widget = Cast<UCreateCharacterWidget>(GameInstance->GetUIMgr()->GetPopupUI(TEXT("CreateCharacter")));
		if (Widget == nullptr)
		{
			return true;
		}
	
		Widget->SetDuplication(pkt.duplication());
	}
	
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ENTER GAME!"));
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_LOAD_SKILL(PacketSessionRef& session, Protocol::S_LOAD_SKILL& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Skill GAME!"));
		GameInstance->HandleLoadSkill(pkt);
	}

	return true;
}

bool Handle_S_LOAD_QUICK(PacketSessionRef& session, Protocol::S_LOAD_QUICK& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Skill GAME!"));
		GameInstance->HandleLoadQuick(pkt);
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		// TODO : 게임 종료? 로비로?
		GameInstance->HandleLeaveGame(pkt);
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}

	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMove(pkt);
	}

	return true;
}

bool Handle_S_ATTACK(PacketSessionRef& session, Protocol::S_ATTACK& pkt)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Packet Recv"));
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleAttack(pkt);
	}

	return true;
}

bool Handle_S_USESKILL(PacketSessionRef& session, Protocol::S_USESKILL& pkt)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Packet Recv"));
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSkill(pkt);
	}

	return true;
}

bool Handle_S_DAMAGED(PacketSessionRef& session, Protocol::S_DAMAGED& pkt)
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged Packet Recv"));
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDamaged(pkt);
	}
	return true;
}

bool Handle_S_DIE(PacketSessionRef& session, Protocol::S_DIE& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDie(pkt);
	}
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();


	return true;
}

bool Handle_S_SETGOLD(PacketSessionRef& session, Protocol::S_SETGOLD& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSetGold(pkt);
	}
	return true;
}

bool Handle_S_ADDITEM(PacketSessionRef& session, Protocol::S_ADDITEM& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleAddItem(pkt);
	}
	return true;
}

bool Handle_S_EQUIP(PacketSessionRef& session, Protocol::S_EQUIP& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleEquipItem(pkt);
	}
	return true;
}

bool Handle_S_SETITEMCOUNT(PacketSessionRef& session, Protocol::S_SETITEMCOUNT& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSetItemCount(pkt);
	}
	return true;
}

bool Handle_S_REMOVEITEM(PacketSessionRef& session, Protocol::S_REMOVEITEM& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleRemoveItem(pkt);
	}
	return true;
}

bool Handle_S_STATCHANGED(PacketSessionRef& session, Protocol::S_STATCHANGED& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSetStatChanged(pkt);
	}
	return true;
}

bool Handle_S_LEVEL_UP(PacketSessionRef& session, Protocol::S_LEVEL_UP& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLevelUp(pkt);
	}
	return true;
}

bool Handle_S_UPGRADESKILL(PacketSessionRef& session, Protocol::S_UPGRADESKILL& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleUpgradeSkill(pkt);
	}
	return true;
}

bool Handle_S_QUEST(PacketSessionRef& session, Protocol::S_QUEST& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleQuest(pkt);
	}
	return true;
}

bool Handle_S_LOAD_TARGET(PacketSessionRef& session, Protocol::S_LOAD_TARGET& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLoadTarget(pkt);
	}
	return true;
}

bool Handle_S_QUICK_QUEST(PacketSessionRef& session, Protocol::S_QUICK_QUEST& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLoadQuickQuest(pkt);
	}
	return true;
}

bool Handle_S_SET_DATA(PacketSessionRef& session, Protocol::S_SET_DATA& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSetData(pkt);
	}
	return true;
}

