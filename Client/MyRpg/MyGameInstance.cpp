// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "Creature/MyPlayer.h"
#include "Creature/RpgPlayer.h"
#include "Monster.h"
#include "Npc.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyPlayerController.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "QuestManager.h"
#include "ActorComponents/InventoryComponent.h"
#include "ActorComponents/SkillComponent.h"
#include "ActorComponents/StatComponent.h"
#include "ActorComponents/QuestComponent.h"
#include "Item.h"
#include "Data/Quest.h"
#include "Skill/Skill.h"
#include "SubItem/QuickSlotWidget.h"

#include "DynamicInfo/QuickQuestWidget.h"
UMyGameInstance::UMyGameInstance()
{


}

void UMyGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance Init"));

	DataMgr = NewObject<UDataManager>(this);
	SceneMgr = NewObject<USceneManager>(this);
	UIMgr = NewObject<UUIManager>(this);
	ResourceMgr = NewObject<UResourceManager>(this);
	SoundMgr = NewObject<USoundManager>(this);
	QuestMgr = NewObject<UQuestManager>(this);

	DataMgr->Init(this);
	UIMgr->Init(this);
	SceneMgr->Init(this);
	ResourceMgr->Init(this);
	SoundMgr->Init(this);
	QuestMgr->Init(this);
}

void UMyGameInstance::ConnectToGameServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Connect To Server"));
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
	
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMyGameInstance::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

}

void UMyGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UMyGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UMyGameInstance::HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, const Protocol::StatInfo& StatInfo, int32 SkillPoint, FString PlayerName, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	auto* World = GetWorld();
	if (World == nullptr)
		return;
	
	// 중복 처리 체크
	const uint64 ObjectId = PlayerInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;
	

	FVector SpawnLocation(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());


	if (PlayerInfo.objtype() == Protocol::ECharacterType::MONSTER)
	{
		SpawnMonster(SpawnLocation, PlayerInfo, StatInfo, PlayerName);
		return;
	}

	if (IsMine)
	{
		InitNpcs();
		SpawnMyPlayer(SpawnLocation, PlayerInfo, StatInfo, PlayerName, SkillPoint);
	}
	else
	{
		SpawnPlayer(SpawnLocation, PlayerInfo, StatInfo, PlayerName);
	}
	
	

}

void UMyGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	FString Name = UTF8_TO_TCHAR(EnterGamePkt.playername().c_str());
	HandleSpawn(EnterGamePkt.player(), EnterGamePkt.stat(), EnterGamePkt.skillpoint(), Name, true);
}

void UMyGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (int i = 0; i < SpawnPkt.players_size(); i++)
	{
		FString Name = UTF8_TO_TCHAR(SpawnPkt.playername().at(i).c_str());
		HandleSpawn(SpawnPkt.players().at(i), SpawnPkt.stats().at(i), -1, Name, false);
	}
}

void UMyGameInstance::HandleLoadSkill(const Protocol::S_LOAD_SKILL& SkillPkt)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Pkt Size : %d"), SkillPkt.skillids_size());
	for (int i = 0; i < SkillPkt.skillids_size(); i++)
	{
		MyPlayer->GetSkillComponent()->SetSkillLevel(SkillPkt.skillids().at(i), SkillPkt.skilllevels().at(i));
	}
}

void UMyGameInstance::HandleLoadQuick(const Protocol::S_LOAD_QUICK& LoadPkt)
{
	for (int i = 0; i < LoadPkt.slot_size(); i++)
	{
		int32 Slot = LoadPkt.slot().at(i);
		int32 Type = LoadPkt.type().at(i);
		uint64 Id = LoadPkt.elementid().at(i);

		if(Type == Protocol::QUICK_ITEM)
		{
			TObjectPtr<UItem> Item = MyPlayer->GetInventory()->GetItem(Id);
			QuickSlots[Slot]->SetItem(Item);
			QuickSlots[Slot]->Refresh();
		}
		if (Type == Protocol::QUICK_SKILL)
		{
			TObjectPtr<USkill> Skill = DataMgr->GetSkill(MyPlayer->GetObjectInfo()->objtype(), Id);
			QuickSlots[Slot]->SetSkill(Skill);
			QuickSlots[Slot]->Refresh();
		}
	}
}

void UMyGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	auto* World = GetWorld();
	if (World == nullptr)
		return;
	
	ARpgPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	
	World->DestroyActor(*FindActor);
}

void UMyGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UMyGameInstance::HandleLeaveGame(const Protocol::S_LEAVE_GAME& LeavePkt)
{
	GameServerSession->Disconnect();
	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}

	UKismetSystemLibrary::QuitGame(GetWorld(), GetFirstLocalPlayerController(), EQuitPreference::Quit, false);
}

void UMyGameInstance::HandleMove(const Protocol::S_MOVE MovePkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}
	
	const uint64 ObjectId = MovePkt.info().object_id();
	const Protocol::ECharacterType ObjType = MovePkt.info().objtype();

	if (ObjectId == MyPlayer->GetObjectInfo()->object_id())
	{
		return;
	}

	ACreature** FindActor = AllCreatures.Find(ObjectId);
	
	if (FindActor == nullptr)
	{
		return;
	}
	if (ObjType != Protocol::ECharacterType::MONSTER)
	{
		ARpgPlayer* Player = Cast<ARpgPlayer>(*FindActor);
		Player->SetObjectInfo(MovePkt.info());
		Player->SetMoveInfo(MovePkt.playerinfo());
	}
	(*FindActor)->SetState(MovePkt.info().state());
	(*FindActor)->SetDestInfo(MovePkt.info());

	
}

void UMyGameInstance::HandleAttack(const Protocol::S_ATTACK AttackPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	const uint64 ObjectId = AttackPkt.info().object_id();
	ACreature* FindActor = *AllCreatures.Find(ObjectId);

	if (MyPlayer->GetObjectInfo()->object_id() == ObjectId)
	{
		return;
	}

	
	if (FindActor == nullptr)
	{
		return;
	}
	FindActor->SetObjectInfo(AttackPkt.info());
	FindActor->SetAttackSequence(AttackPkt.attack_sequence());
	FRotator RotValue(AttackPkt.y(), AttackPkt.z(), AttackPkt.x());
	FindActor->Attack(RotValue);
}

void UMyGameInstance::HandleSkill(const Protocol::S_USESKILL SkillPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	ACreature* FindActor = Cast<ACreature>(*Players.Find(SkillPkt.info().object_id()));
	FindActor->SetObjectInfo(SkillPkt.info());
	ARpgPlayer* Player = Cast<ARpgPlayer>(FindActor);
	Player->UseSkill(SkillPkt.skillid());

	if (Player->IsMyPlayer())
	{
		auto Skill = GetDataMgr()->GetSkill(Player->GetObjectInfo()->objtype(), SkillPkt.skillid());
		Protocol::EQuickSlot QuickSlotKey = Skill->GetQuickSlotKey();
		if (QuickSlotKey != Protocol::QUICK_NONE)
		{
			QuickSlots[QuickSlotKey]->SetCoolDown(SkillPkt.cooldown());
		}
	}
	return;
}

void UMyGameInstance::HandleDamaged(const Protocol::S_DAMAGED DamagedPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	const uint64 ObjectId = DamagedPkt.info().object_id();

	ACreature* FindActor = Cast<ACreature>(*AllCreatures.Find(ObjectId));
	if (FindActor == nullptr)
	{
		return;
	}

	FindActor->OnDamaged(DamagedPkt.damage(), DamagedPkt.attackerid());
}

void UMyGameInstance::HandleDie(const Protocol::S_DIE DiePkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}
	const uint64 ObjectId = DiePkt.info().object_id();
	ACreature* FindActor = Cast<ACreature>(*AllCreatures.Find(ObjectId));
	if (FindActor == nullptr)
	{
		return;
	}

	FindActor->Die();
}

void UMyGameInstance::HandleSetGold(const Protocol::S_SETGOLD& GoldPkt)
{
	MyPlayer->GetInventory()->SetGold(GoldPkt.gold());

	UIMgr->RefreshUI(TEXT("Inventory"));
}

void UMyGameInstance::HandleAddItem(const Protocol::S_ADDITEM& AddPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	TObjectPtr<AMyPlayer> Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->GetInventory()->AddItem(AddPkt.item(), AddPkt.byunequip());


}

void UMyGameInstance::HandleRemoveItem(const Protocol::S_REMOVEITEM RemovePkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	TArray<uint64> DbIds;
	for (int i = 0; i < RemovePkt.itemid_size(); i++)
	{
		DbIds.Add(RemovePkt.itemid().Get(i));
	}
	MyPlayer->GetInventory()->RemoveItem(DbIds);
}

void UMyGameInstance::HandleSetItemCount(const Protocol::S_SETITEMCOUNT CountPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	for(int i = 0; i < CountPkt.itemid_size(); i++)
	{
		MyPlayer->GetInventory()->GetItem(CountPkt.itemid().Get(i))->SetCount(CountPkt.itemcount().Get(i));
		MyPlayer->GetInventory()->GetItem(CountPkt.itemid().Get(i))->SetCountToBeSold(0);
		MyPlayer->GetInventory()->GetItem(CountPkt.itemid().Get(i))->SetToBeSold(false);
		RefreshQuickSlot(MyPlayer->GetInventory()->GetItem(CountPkt.itemid().Get(i))->GetQuickSlotKey());
	}
	
	UIMgr->RefreshUI(TEXT("Inventory"));
	UIMgr->RefreshUI(TEXT("Shop"));
	
	
}

void UMyGameInstance::HandleEquipItem(const Protocol::S_EQUIP& EquipPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}


	TObjectPtr<AMyPlayer> Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->GetInventory()->EquipItem(EquipPkt.equipdbid(), EquipPkt.previtemdbid(), EquipPkt.previtemslotindex());

}

void UMyGameInstance::HandleSetStatChanged(const Protocol::S_STATCHANGED& StatPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	const uint64 ObjectId = StatPkt.object_id();
	ACreature* FindActor = Cast<ACreature>(*AllCreatures.Find(ObjectId));
	if (FindActor == nullptr)
	{
		return;
	}
	FindActor->OnStatChanged(StatPkt.statinfo());
}

void UMyGameInstance::HandleLevelUp(const Protocol::S_LEVEL_UP& LevelUpPkt)
{
	// 1. 해당 플레이어 레벨업 이펙트

	// 2. 근데 만약 그 플레이어가 나라면 스킬 포인트 업데이트
	if (MyPlayer->GetObjectInfo()->object_id() == LevelUpPkt.object_id())
	{
		MyPlayer->GetSkillComponent()->SetSkillPoint(MyPlayer->GetSkillComponent()->GetSkillPoint() + 3);
	}
}

void UMyGameInstance::HandleUpgradeSkill(const Protocol::S_UPGRADESKILL& SkillPkt)
{
	MyPlayer->GetSkillComponent()->SetSkillLevel(SkillPkt.skillid(), SkillPkt.skilllevel());
	MyPlayer->GetSkillComponent()->SetSkillPoint(SkillPkt.leftpoint());
}

void UMyGameInstance::HandleQuest(const Protocol::S_QUEST QuestPkt)
{
	for(int i = 0; i < QuestPkt.questid_size(); i++)
	{
		FQuestData* Data = DataMgr->GetQuestData(QuestPkt.questid().at(i));
		switch (QuestPkt.queststate().at(i))
		{
		case Protocol::QUEST_AVAILABLE:
			QuestMgr->AddAvailableQuest(Data);
			break;
		case Protocol::QUEST_ACCEPT:
			QuestMgr->AcceptQuest(Data);
			break;
		case Protocol::QUEST_ABANDON:
			MyPlayer->GetQuestComponent()->AbandonQuest(QuestPkt.questid().at(i));
			break;
		case Protocol::QUEST_COMPLETABLE:
			QuestMgr->SetQuestCompletable(Data);
			break;
		case Protocol::QUEST_COMPLETED:
			QuestMgr->CompleteQuest(Data);
			break;
		default:
			break;
		}
	}
	UIMgr->RefreshUI(TEXT("PlayerQuest"));
	UIMgr->RefreshUI(TEXT("NpcQuest"));
	UIMgr->RefreshUI(TEXT("NpcQuestInfo"));
}

void UMyGameInstance::HandleLoadTarget(const Protocol::S_LOAD_TARGET Pkt)
{
	for (int i = 0; i < Pkt.questid_size(); i++)
	{
		int32 QuestId = Pkt.questid().at(i);
		int32 TargetIndex = Pkt.targetindex().at(i);
		int32 TargetCount = Pkt.targetcount().at(i);

		MyPlayer->GetQuestComponent()->GetInPrgressQuests()[QuestId]->SetQuestTargetNumber(TargetIndex, TargetCount, this);
	}

	UIMgr->RefreshUI(TEXT("PlayerQuest"));
	UIMgr->RefreshUI(TEXT("NpcQuest"));
	UIMgr->RefreshUI(TEXT("NpcQuestInfo"));
}

void UMyGameInstance::HandleLoadQuickQuest(const Protocol::S_QUICK_QUEST Pkt)
{
	 TObjectPtr<class UQuickQuestWidget> Widget = Cast<UQuickQuestWidget>(UIMgr->GetDynamicUI(TEXT("QeustQuick")));
	if (Widget == nullptr)
	{
		Widget = Cast<UQuickQuestWidget>(UIMgr->PopupDynamicInfoUI(TEXT("QuestQuick")));
	}

	for (int i = 0; i < Pkt.questid_size(); i++)
	{
		Widget->RegisterQuest(Pkt.questid().at(i), this, false);
	}
	
}

void UMyGameInstance::HandleSetData(const Protocol::S_SET_DATA SetPkt)
{
	if (IsConnectedAndValid() == false)
	{
		return;
	}

	const uint64 ObjectId = SetPkt.object_id();
	Protocol::EPlayerData DataType = SetPkt.datatype();

	ACreature* FindActor = Cast<ACreature>(*AllCreatures.Find(ObjectId));
	if (FindActor == nullptr)
	{
		return;
	}

	switch (DataType)
	{
	case Protocol::DATA_ATTACKABLE:
		FindActor->SetCanAttack(true);
		break;
	case Protocol::DATA_UNATTACKABLE:
		FindActor->SetCanAttack(false);
		break;
	case Protocol::DATA_IDLE:
		FindActor->SetState(Protocol::EState::STATE_IDLE);
		FindActor->SetCanAttack(true);
		FindActor->SetAttackSequence(0);
		break;
	default:
		break;
	}
}

void UMyGameInstance::ClickQuickSlot(const Protocol::EQuickSlot Key)
{
	if (Key == Protocol::QUICK_NONE)
	{
		return;
	}
	QuickSlots[Key]->Use(MyPlayer);
}

void UMyGameInstance::RefreshQuickSlot(const Protocol::EQuickSlot Key)
{
	if (Key == Protocol::QUICK_NONE)
	{
		return;
	}
	QuickSlots[Key]->Refresh();
}

void UMyGameInstance::SetQuickSlotNull(const Protocol::EQuickSlot Key)
{
	if (Key == Protocol::QUICK_NONE)
	{
		return;
	}
	QuickSlots[Key]->Empty();
}

/*
		****** Private Funtions ******	
*/
bool UMyGameInstance::IsConnectedAndValid()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return false;

	auto* World = GetWorld();
	if (World == nullptr)
		return false;

	return true;
}

void UMyGameInstance::InitNpcs()
{
	TArray<AActor*> FoundNpcs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANpc::StaticClass(), FoundNpcs);


	for (AActor* Actor : FoundNpcs)
	{
		if (ANpc* Npc = Cast<ANpc>(Actor)) // 안전하게 캐스팅
		{
			// 원하는 로직 추가
			UE_LOG(LogTemp, Warning, TEXT("Found NPC: %s"), *Npc->GetName());
			Npc->Init(); // Init 호출
		}
	}
}

void UMyGameInstance::SpawnMonster(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const StatInfo& StatInfo, FString TemplateId)
{
	FString Name = TEXT("BP_Monster_") + TemplateId + TEXT(".BP_Monster_") + TemplateId + TEXT("_C");
	TObjectPtr<AMonster> Monster = Cast<AMonster>(ResourceMgr->InstantiateActor(TEXT("Creature/Monsters/"), Name, SpawnLocation));

	if (Monster == nullptr)
	{
		return;
	}
	
	Monster->Init(ObjInfo, StatInfo, TEXT(""));
	Monsters.Add(ObjInfo.object_id(), Monster);
	AllCreatures.Add(ObjInfo.object_id(), Monster);
	
	return;
}

void UMyGameInstance::SpawnPlayer(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const StatInfo& StatInfo, FString PlayerName)
{
	TObjectPtr<ARpgPlayer> Player = Cast<ARpgPlayer>(GetWorld()->SpawnActor(OhterPlayerClass, &SpawnLocation));
	if (Player == nullptr)
	{
		return;
	}
	Player->Init(ObjInfo, StatInfo, PlayerName);
	Player->SetHpBarVisibility(false);
	Players.Add(ObjInfo.object_id(), Player);
	AllCreatures.Add(ObjInfo.object_id(), Player);
}

void UMyGameInstance::SpawnMyPlayer(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const StatInfo& StatInfo, FString PlayerName, int32 SkillPoint)
{
	TObjectPtr<AMyPlayer> Player = Cast<AMyPlayer>(GetWorld()->SpawnActor(MyPlayerClass, &SpawnLocation));
	
	if (Player == nullptr)
	{
		return;
	}
	MyPlayer = Player;
	
	/* 플레이어 컨트롤러 부여 */
	auto* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->Possess(Player);
	Cast<AMyPlayerController>(PC)->SetMyPlayer(Player);
	
	
	Player->Init(ObjInfo, StatInfo, PlayerName);
	Player->InitSkill(SkillPoint);
	
	Players.Add(ObjInfo.object_id(), Player);
	AllCreatures.Add(ObjInfo.object_id(), Player);
	
	UIMgr->InitializeSceneWidget();
}


