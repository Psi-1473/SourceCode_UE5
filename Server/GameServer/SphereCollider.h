#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider() override;

public:
	virtual bool IsColliding(shared_ptr<Collider> ohter) override;

	void SetRadius(float value) { radius = value; }
	float GetRadius() { return radius; }

private:
	float radius;
};

