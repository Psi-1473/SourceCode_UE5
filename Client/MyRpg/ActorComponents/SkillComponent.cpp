#include "ActorComponents/SkillComponent.h"
#include "MyRpg.h"
#include "Protocol.pb.h"
#include "MyPlayer.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USkillComponent::Init(TObjectPtr<AMyPlayer> Player)
{
	OwnerPlayer = Player;
}

void USkillComponent::SendUpgradeSkillPacket(int32 SkillId)
{
	Protocol::C_UPGRADESKILL Pkt;
	Pkt.set_object_id(OwnerPlayer->GetObjectInfo()->object_id());
	Pkt.set_skillid(SkillId);

	SEND_PACKET(Pkt);
}

void USkillComponent::SetSkillPoint(int32 Point)
{
	SkillPoint = Point;

	OnSkillUpdated.Broadcast();
}

void USkillComponent::SetSkillLevel(int32 SkillId, int32 SkillLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("Set Skill %d(%d)"), SkillId, SkillLevel);
	if (SkillList.Find(SkillId) == nullptr)
	{
		SkillList.Add(SkillId, SkillLevel);
	}
	else
	{
		SkillList[SkillId] = SkillLevel;
	}

	OnSkillUpdated.Broadcast();
}

int32 USkillComponent::GetSkillLevel(int32 SkillId)
{
	if (SkillList.Find(SkillId) == nullptr)
	{
		return 0;
	}
	else
	{
		return SkillList[SkillId];
	}
}
