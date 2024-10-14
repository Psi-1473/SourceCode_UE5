#pragma once

#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

class BoxCollider;
class SphereCollider;
class ColliderUtils
{
public:
	static shared_ptr<Collider> CreateCollider(Vec3 center, Vec3 size, Vec3 rotation, string hitType);
	static shared_ptr<BoxCollider> CreateBoxCollider(Vec3 center, Vec3 size, Vec3 rotation);
	static shared_ptr<SphereCollider> CreateSphereCollider(Vec3 center, float radius, float verticalSkipRange);


	// Check Collision
	static bool IsCollidingBoxSphere(shared_ptr<BoxCollider> box, shared_ptr<SphereCollider> sphere);
	static bool IsCollidingBoxies(shared_ptr<BoxCollider> box1, shared_ptr<BoxCollider> box2);
	static bool IsCollidingSpheres(shared_ptr<SphereCollider> sphere1, shared_ptr<SphereCollider> sphere2);


private:
	/*
		Box Collider끼리의 충돌을 판별할 때 사용할 함수
	*/
	static bool IsOverlapping(const vector<Vec3> vertices1, const vector<Vec3> vertices2, const Vec3& axis);
	static pair<double, double> ProjectVertices(const vector<Vec3>& vertices, const Vec3& axis);

};

