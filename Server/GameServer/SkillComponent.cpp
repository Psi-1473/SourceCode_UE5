#include "pch.h"
#include "SkillComponent.h"
#include "Player.h"
#include "QuickSlotComponent.h"

#include "iostream"

void SkillComponent::InitWithPlayerStart(const vector<SkillInfo>& newSkills)
{
	Protocol::S_LOAD_SKILL pkt;
	for (int i = 0; i < newSkills.size(); i++)
	{
		skills[newSkills[i].skillId] = newSkills[i];
		skillUsable[newSkills[i].skillId] = true;
		if (newSkills[i].quickSlot != Protocol::QUICK_NONE)
		{
			owner.lock()->GetQuickSlotComponent()->SetElement(newSkills[i].quickSlot, Protocol::QUICK_SKILL, newSkills[i].skillId);
		}

		pkt.add_skillids(newSkills[i].skillId);
		pkt.add_skilllevels(newSkills[i].skillLevel);
	}

	cout << "Skill Set Size : " << pkt.skillids_size() << endl;
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);

	owner.lock()->GetQuickSlotComponent()->InitWithPlayerStart();
}

void SkillComponent::Init(shared_ptr<class Player> player)
{
	owner = player;
}

void SkillComponent::SetSkillLevel(int skillId, int skillLevel)
{
	if (skills.count(skillId) <= 0)
	{
		SkillInfo info;
		info.skillId = skillId;
		info.skillLevel = 0;
		info.quickSlot = Protocol::QUICK_NONE;
		skillUsable[skillId] = true;
		skills[skillId] = info;
	}

	skills[skillId].skillLevel = skillLevel;

	skillPoint--;
	SendUpgradeSkillPacket(skillId);

}

void SkillComponent::LevelUp()
{
	skillPoint += 3;
}



void SkillComponent::SendUpgradeSkillPacket(int skillId)
{
	Protocol::S_UPGRADESKILL pkt;

	pkt.set_skillid(skillId);
	pkt.set_skilllevel(skills[skillId].skillLevel);
	pkt.set_leftpoint(skillPoint);

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	owner.lock()->session.lock()->Send(sendBuffer);
}
