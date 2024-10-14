// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyRpg.h"
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class MYRPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();

	void Init();
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	void HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, const Protocol::StatInfo& StatInfo, int32 SkillPoint, FString PlayerName, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleLoadSkill(const Protocol::S_LOAD_SKILL& SkillPkt);
	void HandleLoadQuick(const Protocol::S_LOAD_QUICK& LoadPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	void HandleLeaveGame(const Protocol::S_LEAVE_GAME& LeavePkt);

	void HandleMove(const Protocol::S_MOVE MovePkt);
	void HandleAttack(const Protocol::S_ATTACK AttackPkt);
	void HandleSkill(const Protocol::S_USESKILL SkillPkt);
	void HandleDamaged(const Protocol::S_DAMAGED DamagedPkt);
	void HandleDie(const Protocol::S_DIE DiePkt);

	void HandleSetGold(const Protocol::S_SETGOLD& GoldPkt);
	void HandleAddItem(const Protocol::S_ADDITEM& AddPkt);
	void HandleRemoveItem(const Protocol::S_REMOVEITEM RemovePkt);
	void HandleSetItemCount(const Protocol::S_SETITEMCOUNT CountPkt);
	void HandleEquipItem(const Protocol::S_EQUIP& EquipPkt);

	void HandleSetStatChanged(const Protocol::S_STATCHANGED& StatPkt);
	void HandleLevelUp(const Protocol::S_LEVEL_UP& LevelUpPkt);
	void HandleUpgradeSkill(const Protocol::S_UPGRADESKILL& SkillPkt);
	void HandleQuest(const Protocol::S_QUEST QuestPkt);
	void HandleLoadTarget(const Protocol::S_LOAD_TARGET Pkt);
	void HandleLoadQuickQuest(const Protocol::S_QUICK_QUEST Pkt);

	void HandleSetData(const Protocol::S_SET_DATA SetPkt);
public:
	void ClickQuickSlot(const Protocol::EQuickSlot Key);
	void RefreshQuickSlot(const Protocol::EQuickSlot Key);
	void SetQuickSlotNull(const Protocol::EQuickSlot Key);

	
public:
	/* Get, Set*/
	TWeakObjectPtr<class AMyPlayer> GetPlayer() { return GPlayer; }
	TObjectPtr<class UDataManager> GetDataMgr() { return DataMgr; }
	TObjectPtr<class UUIManager> GetUIMgr() { return UIMgr; }
	TObjectPtr<class USceneManager> GetSceneMgr() { return SceneMgr; }
	TObjectPtr<class UResourceManager> GetResourceMgr() { return ResourceMgr; }
	TObjectPtr<class USoundManager> GetSoundMgr() { return SoundMgr; }
	TObjectPtr<class UQuestManager> GetQuestMgr() { return QuestMgr; }


	class ANpc* GetNpc(int32 NpcId) { return Npcs[NpcId]; }


	void SetGPlayer(TWeakObjectPtr<class AMyPlayer> Player) { GPlayer = Player; }
private:
	bool IsConnectedAndValid();

	void InitNpcs();

	void SpawnMonster(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString TemplateId);
	void SpawnPlayer(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString PlayerName);
	void SpawnMyPlayer(const FVector& SpawnLocation, const Protocol::ObjectInfo& ObjInfo, const Protocol::StatInfo& StatInfo, FString PlayerName, int32 SkillPoint);
public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

	uint64 UserDBId;
	uint64 CharacterDBId;


public:
	// Manager 클래스
	UPROPERTY()
	TObjectPtr<class UDataManager> DataMgr;

	UPROPERTY()
	TObjectPtr<class UUIManager> UIMgr;

	UPROPERTY()
	TObjectPtr<class USceneManager> SceneMgr;

	UPROPERTY()
	TObjectPtr<class UResourceManager> ResourceMgr;

	UPROPERTY()
	TObjectPtr<class USoundManager> SoundMgr;

	UPROPERTY()
	TObjectPtr<class UQuestManager> QuestMgr;
public:
	// Room 역할을 위한 변수들
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMyPlayer> MyPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARpgPlayer> OhterPlayerClass;

	UPROPERTY()
	class AMyPlayer* MyPlayer;

	UPROPERTY()
	TMap<uint64, class ACreature*> AllCreatures;

	UPROPERTY()
	TMap<uint64, class ARpgPlayer*> Players;

	UPROPERTY()
	TMap<uint64, class AMonster*> Monsters;

	UPROPERTY()
	TMap<int32, class ANpc*> Npcs;

	UPROPERTY()
	TWeakObjectPtr<class AMyPlayer> GPlayer;

	// Quick Slot
	UPROPERTY()
	TMap<int32, TObjectPtr<class UQuickSlotWidget>> QuickSlots;

public:


};
