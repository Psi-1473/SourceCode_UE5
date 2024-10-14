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

	// Enum �� �߰��ؼ� �� ������ � ������ �ٲ�� �� �˸���
	void SendStatPacket();
public:
	shared_ptr<Protocol::StatInfo> GetStatInfo() { return statInfo; }
protected:
	// ���� ������ �ִ�ġ ���� (MaxHp, MaxMp ...)
	shared_ptr<struct BaseStatInfo> defaultStat;

	// ���� ���� ���� ���� (Hp, Mp ...)
	shared_ptr<Protocol::StatInfo> statInfo;

	weak_ptr<class Creature> owner;
};

