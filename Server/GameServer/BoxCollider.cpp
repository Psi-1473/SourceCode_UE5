#include "pch.h"
#include "BoxCollider.h"
#include "MatrixHelper.h"
#include "ColliderUtils.h"
#include <algorithm>

BoxCollider::BoxCollider()
{
	type = ColliderType::Box;
	centerPoint = Vec3(0.f, 0.f, 0.f);
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::IsColliding(shared_ptr<Collider> other)
{
	bool ret = false;
	switch (other->GetType())
	{
		case ColliderType::Box:
			ret = ColliderUtils::IsCollidingBoxies(static_pointer_cast<BoxCollider>(shared_from_this()), static_pointer_cast<BoxCollider>(other));
			break;
	}

	return ret;
}

vector<Vec3> BoxCollider::GetLocalVerticesWithNoRotation()
{
	/*
		Box�� ���� ��ǥ 
	*/
	std::vector<Vec3> vertices;
	float halfX = size.x / 2;
	float halfY = size.y / 2;
	float halfZ = size.z / 2;

	vertices.push_back(Vec3(-halfX, -halfY, -halfZ));
	vertices.push_back(Vec3(halfX, -halfY, -halfZ));
	vertices.push_back(Vec3(halfX, halfY, -halfZ));
	vertices.push_back(Vec3(-halfX, halfY, -halfZ));
	vertices.push_back(Vec3(-halfX, -halfY, halfZ));
	vertices.push_back(Vec3(halfX, -halfY, halfZ));
	vertices.push_back(Vec3(halfX, halfY, halfZ));
	vertices.push_back(Vec3(-halfX, halfY, halfZ));

	return vertices;
}

vector<Vec3> BoxCollider::GetWorldVertices()
{
	// ���� ��ǥ�� ȸ�� ���� ��ġ ���� ������ ���� ��ǥ
	std::vector<Vec3> localVertices = GetLocalVerticesWithNoRotation();
	std::vector<Vec3> worldVertices;
	for (const auto& vertex : localVertices) {
		Vec3 worldVertex = GetWorldVertex(vertex) + centerPoint;
		worldVertices.push_back(worldVertex);
	}
	return worldVertices;
}

array<Vec3, 3> BoxCollider::GetFaceNormals()
{
	/*
		����ü ����� �븻 ���� ���ϴ� �Լ�
	*/
	std::array<Vec3, 3> faceNormals = {
			Vec3(1, 0, 0), // Local X axis
			Vec3(0, 1, 0), // Local Y axis
			Vec3(0, 0, 1)  // Local Z axis
	};

	for (auto& normal : faceNormals) {
		normal = GetWorldVertex(normal);
		normal = normal.normalize();
	}

	return faceNormals;
}

// �� �ٲ㰡�鼭 �浹�ϳ� �Ǻ��ϱ�
Vec3 BoxCollider::GetWorldVertex(Vec3 localPos)
{
	array<array<double, 4>, 4> rotMatrix = MatrixHelper::GetRotationMatrix(rotation);

	Vec3 worldVertex = MatrixHelper::ApplyMatrix(rotMatrix, localPos);

	return worldVertex;
}

Vec3 BoxCollider::GetLocalMin()
{
	vector<Vec3> localVertices = GetLocalVerticesWithNoRotation();
	float xMin = localVertices[0].x;
	float yMin = localVertices[0].y;
	float zMin = localVertices[0].z;

	for (int i = 1; i < localVertices.size(); i++)
	{
		xMin = min(xMin, localVertices[i].x);
		yMin = min(yMin, localVertices[i].y);
		zMin = min(zMin, localVertices[i].z);
	}

	return Vec3(xMin, yMin, zMin);
}

Vec3 BoxCollider::GetLocalMax()
{
	vector<Vec3> localVertices = GetLocalVerticesWithNoRotation();
	float xMax = localVertices[0].x;
	float yMax = localVertices[0].y;
	float zMax = localVertices[0].z;

	for (int i = 1; i < localVertices.size(); i++)
	{
		xMax = max(xMax, localVertices[i].x);
		yMax = max(yMax, localVertices[i].y);
		zMax = max(zMax, localVertices[i].z);
	}

	return Vec3(xMax, yMax, zMax);
}