#pragma once
#include <array>
#include "Geometry.h"

using namespace std;



enum class ColliderType
{
	Box,
	Capsule,
	Sphere,
};

class Collider : public enable_shared_from_this<Collider>
{
public:
	Collider();
	virtual ~Collider();

public:
	ColliderType GetType() const { return type; }
	const Vec3& GetCenterPoint() { return centerPoint; }
	void SetCenterPoint(Vec3 value)  { centerPoint = value; }

public:
	virtual bool IsColliding(shared_ptr<Collider> other) = 0;

protected:
	Vec3 centerPoint;
	ColliderType type;
};

