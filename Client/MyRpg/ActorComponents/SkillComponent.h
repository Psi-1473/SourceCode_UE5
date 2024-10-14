#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnSkillUpdated)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPG_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();
	void Init(TObjectPtr<class AMyPlayer> Player);

	void SendUpgradeSkillPacket(int32 SkillId);

	void SetSkillPoint(int32 Point);
	void SetSkillLevel(int32 SkillId, int32 SkillLevel);

	int32 GetSkillPoint() { return SkillPoint; }
	int32 GetSkillLevel(int32 SkillId);
public:
	FOnSkillUpdated OnSkillUpdated;
private:
	// SkillId, Skill Level
	UPROPERTY()
	TMap<int32, int32> SkillList;

	UPROPERTY()
	TWeakObjectPtr<class AMyPlayer> OwnerPlayer;

	int32 SkillPoint = 5;


};
