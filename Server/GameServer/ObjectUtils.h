#pragma once

class ObjectUtils
{
public:
	static PlayerRef CreatePlayer(GameSessionRef session);
	static MonsterRef CreateMonster(int templateId);
	static ItemRef CreateItem(Protocol::EItemType type, uint64 dataId, uint32 templateId, uint32 count);

private:
	static atomic<int64> s_idGenerator;
};

