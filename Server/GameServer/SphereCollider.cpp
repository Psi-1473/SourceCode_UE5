#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "math.h"
#include "MatrixHelper.h"
#include "ColliderUtils.h"
#include <numbers> // For std::numbers::pi

SphereCollider::SphereCollider()
{
	type = ColliderType::Sphere;
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsColliding(shared_ptr<Collider> other)
{
	bool ret = false;
	auto mySphere = static_pointer_cast<SphereCollider>(shared_from_this());

	switch (other->GetType())
	{
	case ColliderType::Box:
		ret = ColliderUtils::IsCollidingBoxSphere(static_pointer_cast<BoxCollider>(other), mySphere);

		break;
	case ColliderType::Sphere:
		ret = ColliderUtils::IsCollidingSpheres(static_pointer_cast<SphereCollider>(other), mySphere);
		break;
	}

	return ret;
}