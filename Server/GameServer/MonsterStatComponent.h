#pragma once
#include "StatComponent.h"

class MonsterStatComponent : public StatComponent
{
public:
	virtual void SetDefaultStat(int32 levelOrId) override;
	virtual void Init(CreatureRef creature, int32 level) override;
};

