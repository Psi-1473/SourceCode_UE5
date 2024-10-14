#pragma once

class AttackComponent
{
public:
	AttackComponent();
	~AttackComponent();
public:
	void Init(CreatureRef creature) { owner = creature; }
public:

	void PlayerAttack(const Vec3& rotation, int32 sequence);
	void MonsterAttack(int32 monsterId, const Vec3& rotation, int32 sequence);
	void UseSkill(int32 skillId, int32 skillLevel, int32 mp, const Vec3& rotation);

public:
	int32 GetAttackSequence() { return attackSequence; }
	bool GetCanAttack() { return canAttack; }

	void SetAttackSequence(int32 value) { attackSequence = value; }
	void SetCanAttack(bool value) { canAttack = value; }

private:
	bool IsSkillUsable(int32 skillId, int32 mp);
	bool IsAttackable(shared_ptr<struct CharacterAttackInfo> info, int32 sequence);

	void HitCheck(const unordered_map<int, shared_ptr<struct SkillHitInfo>>& info, int32 skillId, int32 skillLevel, Vec3 rotation);
	void HitCheck(shared_ptr<struct CharacterAttackInfo> info, const Vec3& rotation, int32 sequence);

	void ApplySkillCooldown(int32 skillId, int32 coolDown);


	void SendAttackPacket(int attackSequence, const Vec3& rotation);
	void SendSkillPacket(int skillId);

private:
	int32 GetMaxCombo();



private:
	int32 attackSequence;
	bool canAttack;
private:
	weak_ptr<class Creature> owner;
};

