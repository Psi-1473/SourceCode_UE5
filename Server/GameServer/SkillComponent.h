#pragma once



struct SkillInfo
{
	int32 skillId;
	int32 skillLevel;
	Protocol::EQuickSlot quickSlot;
};
class SkillComponent
{

public:
	/* Component ��ü�� �ʱ�ȭ �Լ� */
	void Init(shared_ptr<class Player> player);

	/* �÷��̾ �������� �� ������ �ش� �÷��̾��� ��ų ���� �ʱ�ȭ �Լ� */
	void InitWithPlayerStart(const vector<SkillInfo>& newSkills);

	/* �÷��̾��� ��ų �����̳� ��ų ����Ʈ�� �����ϱ� ���� �Լ�*/
	void SetSkillLevel(int skillId, int skillLevel);
	void SetSkillPoint(int point) { skillPoint = point; }
	void SetSkillUsable(int skillId, bool value) { skillUsable[skillId] = value; }
	void LevelUp();
public:
	/* Get Functions */
	int GetSkillPoint() { return skillPoint; }
	int GetSkillLevel(int skillId) { if(skills.count(skillId) <= 0) return 0; return skills[skillId].skillLevel; }
	bool GetSkillUsable(int skillId) { return skillUsable[skillId]; }

private:
	/* ��ų ������ ����Ǿ��� �� ��Ŷ ���� �Լ� */
	void SendUpgradeSkillPacket(int skillId);


private:
	// Key - Skill Id, Value - Skill Level
	unordered_map<int, SkillInfo> skills;
	unordered_map<int, bool> skillUsable;
	weak_ptr<class Player> owner;

	int skillPoint = 5;
};

