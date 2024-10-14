#pragma once
#include "StatComponent.h"

class PlayerStatComponent : public StatComponent
{
public:
	virtual void SetDefaultStat(int32 levelOrId) override;
	virtual void Init(CreatureRef creature, int32 level) override;
public:
	void IncreaseExp(int value);
	void DecreaseExp(int value);
private:
	void LevelUp(CreatureRef creature, int32 level);

private:
	//void SendLevelUpPacket();
};

