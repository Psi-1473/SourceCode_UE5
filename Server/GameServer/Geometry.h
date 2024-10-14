#pragma once



struct Vec3
{
	float x; // 좌우
	float y; // 앞뒤
	float z; // 위아래

	Vec3() : x(0), y(0), z(0) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vec3(const Protocol::ObjectInfo& playerInfo)
	{
		x = playerInfo.x();
		y = playerInfo.y();
		z = playerInfo.z();
	}

	Vec3 operator*(float value) const {return Vec3(x * value, y * value, z * value); }
	Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	Vec3 cross(const Vec3& v) const {
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}
	float length() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3 normalize() const
	{
		float len = length();
		if (len > 0) {
			return Vec3(x / len, y / len, z / len);
		}
		return Vec3();
	}

	float distanceTo(const Vec3& v) const { return std::sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z)); }
	float squaredDistance(const Vec3& v) const { return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z); }
	Vec3 UE_Vec() { return Vec3(x, z, y); }
};

struct Vec2
{
	float x;
	float y;

	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}
	Vec2(const Protocol::ObjectInfo& playerInfo)
	{
		x = playerInfo.x();
		y = playerInfo.y();
	}
};

struct Vec2Int
{
	int x;
	int y;

	Vec2Int() : x(0), y(0) {}
	Vec2Int(int x, int y) : x(x), y(y) {}

	Vec2Int operator+(const Vec2Int& v) const { return Vec2Int(x + v.x, y + v.y); }
	bool operator ==(const Vec2Int& v) const { return (x == v.x && y == v.y); }
	bool operator !=(const Vec2Int& v) const { return (x != v.x || y != v.y); }
};




namespace std {
	template <>
	struct hash<Vec2Int> {
		std::size_t operator()(const Vec2Int& v) const {
			// 해시 시드를 추가하여 충돌을 줄인다.
			std::size_t seed = 0;
			// x와 y 값을 해싱하고 시드를 사용하여 조합
			seed ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}