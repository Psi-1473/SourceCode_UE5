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
	/* Component 자체의 초기화 함수 */
	void Init(shared_ptr<class Player> player);

	/* 플레이어가 입장했을 때 실행할 해당 플레이어의 스킬 정보 초기화 함수 */
	void InitWithPlayerStart(const vector<SkillInfo>& newSkills);

	/* 플레이어의 스킬 레벨이나 스킬 포인트를 설정하기 위한 함수*/
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
	/* 스킬 정보가 변경되었을 때 패킷 전송 함수 */
	void SendUpgradeSkillPacket(int skillId);


private:
	// Key - Skill Id, Value - Skill Level
	unordered_map<int, SkillInfo> skills;
	unordered_map<int, bool> skillUsable;
	weak_ptr<class Player> owner;

	int skillPoint = 5;
};

