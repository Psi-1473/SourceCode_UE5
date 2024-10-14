#include "pch.h"
#include "DataManager.h"

shared_ptr<DataManager> GDataManager = make_shared<DataManager>();
void DataManager::Init()
{
	
}

const NESTED_UNORDERED_MAP(int, int, shared_ptr<SkillHitInfo>)& DataManager::GetSkillHitInfo(Protocol::ECharacterType type)
{
	switch (type)
	{
		case Protocol::WARRIOR:
			return warriorSkillHitData;
			break;
		case Protocol::ROGUE:
			return rogueSkillHitData;
			break;
		case Protocol::WIZARD:
			return wizardSkillHitData;
			break;
		default:
			return warriorSkillHitData;
			break;
	}

	//default는 일단 임시로 저거 해놓음
}
