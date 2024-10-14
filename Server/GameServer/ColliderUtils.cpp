#include "pch.h"
#include "ColliderUtils.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "MatrixHelper.h"

shared_ptr<Collider> ColliderUtils::CreateCollider(Vec3 center, Vec3 size, Vec3 rotation, string hitType)
{
	if (hitType == "Box" || hitType == "BOX")
		return ColliderUtils::CreateBoxCollider(center, size, rotation);
	if (hitType == "Sphere")
		return ColliderUtils::CreateSphereCollider(center, size.x, size.z);

	return nullptr;
}

shared_ptr<BoxCollider> ColliderUtils::CreateBoxCollider(Vec3 center, Vec3 size, Vec3 rotation)
{
	shared_ptr<BoxCollider> boxCollider = make_shared<BoxCollider>();

	boxCollider->SetCenterPoint(center);
	boxCollider->SetRotation(rotation);
	boxCollider->SetSize(size);

	return boxCollider;
}

shared_ptr<SphereCollider> ColliderUtils::CreateSphereCollider(Vec3 center, float radius, float verticalSkipRange)
{
	shared_ptr<SphereCollider> sphereCollider = make_shared<SphereCollider>();
	sphereCollider->SetCenterPoint(center);
	sphereCollider->SetRadius(radius);
	// vertical 나중에 추가
	return sphereCollider;
}

bool ColliderUtils::IsCollidingBoxSphere(shared_ptr<BoxCollider> box, shared_ptr<SphereCollider> sphere)
{
	Vec3 boxCenter = box->GetCenterPoint();
	Vec3 sphereCenter = sphere->GetCenterPoint();

	array<array<double, 4>, 4> rotationMatrix = MatrixHelper::GetRotationMatrix(box->GetRotation());
	Vec3 boxToSphere = sphereCenter - boxCenter;
	Vec3 localSphereCenter = MatrixHelper::ApplyMatrix(MatrixHelper::Transpose(rotationMatrix), boxToSphere);

	Vec3 boxMinLocal = box->GetLocalMin();
	Vec3 boxMaxLocal = box->GetLocalMax();


	Vec3 closestPoint;
	closestPoint.x = max(boxMinLocal.x, min(localSphereCenter.x, boxMaxLocal.x));
	closestPoint.y = max(boxMinLocal.y, min(localSphereCenter.y, boxMaxLocal.y));
	closestPoint.z = max(boxMinLocal.z, min(localSphereCenter.z, boxMaxLocal.z));

	float distSquared = localSphereCenter.squaredDistance(closestPoint);

	double radius = sphere->GetRadius();
	return distSquared <= radius * radius;
}

bool ColliderUtils::IsCollidingBoxies(shared_ptr<BoxCollider> box1, shared_ptr<BoxCollider> box2)
{
	std::array<Vec3, 3> normals1 = box1->GetFaceNormals();
	std::array<Vec3, 3> normals2 = box2->GetFaceNormals();

	std::vector<Vec3> vertices1 = box1->GetWorldVertices();
	std::vector<Vec3> vertices2 = box2->GetWorldVertices();

	for (const auto& normal : normals1) {
		if (!IsOverlapping(vertices1, vertices2, normal)) {
			return false;
		}
	}

	for (const auto& normal : normals2) {
		if (!IsOverlapping(vertices1, vertices2, normal)) {
			return false;
		}
	}

	return true;
}

bool ColliderUtils::IsCollidingSpheres(shared_ptr<SphereCollider> sphere1, shared_ptr<SphereCollider> sphere2)
{
	float distance = sphere1->GetCenterPoint().distanceTo(sphere2->GetCenterPoint());
	float sumOfRadius = sphere1->GetRadius() + sphere2->GetRadius();


	return (distance <= sumOfRadius) ? true : false;
}

bool ColliderUtils::IsOverlapping(const vector<Vec3> vertices1, const vector<Vec3> vertices2, const Vec3& axis)
{
	// 두 상자의 정점을 주어진 축에 투영합니다.
	auto [min1, max1] = ProjectVertices(vertices1, axis);
	auto [min2, max2] = ProjectVertices(vertices2, axis);

	// 투영된 선분이 겹치는지 확인합니다.
	return !(max1 < min2 || max2 < min1);
}

pair<double, double> ColliderUtils::ProjectVertices(const vector<Vec3>& vertices, const Vec3& axis)
{
	double min = axis.dot(vertices[0]);
	double max = min;

	for (const auto& vertex : vertices) {
		double projection = axis.dot(vertex);

		if (projection < min) {
			min = projection;
		}
		if (projection > max) {
			max = projection;
		}
	}

	return { min, max };
}
