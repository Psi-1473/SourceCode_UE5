#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	// ID »ý¼º±â
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->GetObjectInfo()->set_object_id(newId);
	player->session = session;
	session->player.store(player);

	return player;
}

MonsterRef ObjectUtils::CreateMonster(int templateId)
{
	const int64 newId = s_idGenerator.fetch_add(1);

	MonsterRef monster = make_shared<Monster>();
	monster->GetObjectInfo()->set_object_id(newId);
	monster->SetTemplateId(templateId);
	return monster;
}

ItemRef ObjectUtils::CreateItem(Protocol::EItemType type, uint64 dataId, uint32 templateId, uint32 count)
{
	ItemRef item;
	switch (type)
	{
		case Protocol::ITEM_WEAPON:
			item = make_shared<Weapon>(dataId, templateId, count);
			break;
		case Protocol::ITEM_ARMOR:
			item = make_shared<Armor>(dataId, templateId, count);
			break;
		case Protocol::ITEM_CONSUMABLE:
			item = make_shared<Consumable>(dataId, templateId, count);
			break;
	}
	item->SetQuickType(Protocol::QUICK_NONE);
	item->SetCount(count);
	return item;
}
