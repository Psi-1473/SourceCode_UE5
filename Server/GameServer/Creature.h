#pragma once
#include "BaseObject.h"
#include "Geometry.h"

class Creature : public BaseObject
{
public:
	Creature();
	virtual ~Creature();

public:
	virtual void Attack(CreatureRef target);
	virtual void OnDamaged(float damage, CreatureRef attacker);
	virtual void Die(CreatureRef attacker);

public:
	/* Send Packet Functions*/
	void SendStatPacket();
	void SendAttackPacket(int attackSequence, Vec3 rotation);

protected:

public:
	shared_ptr<class StatComponent> GetStatComponent() { return statComponent; }
	shared_ptr<class AttackComponent> GetSAttackComponent() { return attackComponent; }

protected:
	shared_ptr<class StatComponent> statComponent;
	shared_ptr<class AttackComponent> attackComponent;
	// temp
	float damagedToIdleTick;
};

