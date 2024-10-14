#pragma once
#include "Collider.h"


class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider() override;

public:
	virtual bool IsColliding(shared_ptr<Collider> other) override;


	void SetSize(Vec3 value) { size = value; }
	void SetRotation(Vec3 value) { rotation = value; }

	Vec3 GetLocalMin();
	Vec3 GetLocalMax();
	Vec3 GetRotation() { return rotation; }
public:
	vector<Vec3> GetLocalVerticesWithNoRotation();
	vector<Vec3> GetWorldVertices();
	array<Vec3, 3> GetFaceNormals();

	Vec3 GetWorldVertex(Vec3 localPos);

private:
	Vec3 size;
	Vec3 rotation;
	
};

