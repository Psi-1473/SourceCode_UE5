#pragma once
class StatComponent
{
public:
	StatComponent();
	
	virtual void Init(CreatureRef creature, int32 level);
	virtual void SetDefaultStat(int32 levelOrId);

	void IncreaseHp(int value);
	void DecreaseHp(int value, CreatureRef attacker);
	void IncreaseMp(int value);
	void DecreaseMp(int value);

	void IncreaseAtk(int value);
	void DecreaseAtk(int value);
	void IncreaseDfs(int value);
	void DecreaseDfs(int value);

	// Enum 값 추가해서 그 값으로 어떤 스탯이 바뀌는 지 알리자
	void SendStatPacket();
public:
	shared_ptr<Protocol::StatInfo> GetStatInfo() { return statInfo; }
protected:
	// 각종 스탯의 최대치 정보 (MaxHp, MaxMp ...)
	shared_ptr<struct BaseStatInfo> defaultStat;

	// 각종 스탯 현재 정보 (Hp, Mp ...)
	shared_ptr<Protocol::StatInfo> statInfo;

	weak_ptr<class Creature> owner;
};

