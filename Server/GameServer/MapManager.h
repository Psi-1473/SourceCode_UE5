#pragma once
#include "Geometry.h"


struct NodeInfo
{
	Vec2Int gridPos;

	float G;
	float H;
	int depth;
	float F() const { return (G + H); }
	NodeInfo(Vec2Int gridpos, float g, float h, int depth) : gridPos(gridpos), G(g), H(h), depth(depth) {};
};

struct CompareNode
{
	bool operator()(const NodeInfo& n1, const NodeInfo& n2)
	{
		return (n1.G + n1.H) > (n2.G + n2.H);
	}
};
class MapManager
{
public:
	void OccupyGrid(const Vec3& pos);
	void ReleaseGrid(const Vec3& pos);
public:
	void LoadHeightMap();
	void LoadMovableMap();
	void InitializeMobInfo();

	vector<Vec3> FindPath(Vec3 startPos, Vec3 destPos);
	vector<Vec3> CalcPathFromParent(unordered_map<Vec2Int, Vec2Int> parent, Vec2Int dest, Vec2Int start);

	bool isMovablePlace(const Vec2Int& gridPos);

	// Æ¯Á¤ ÁÂÇ¥¸¦ ÅëÇØ Grid ÁÂÇ¥¸¦ ¾ò¾î³¿
	const Vec2Int& GetGridPosFromObjPos(const Vec3& objPos);
	
	// Æ¯Á¤ ±×¸®µå ÁÂÇ¥¸¦ World ÁÂÇ¥·Î
	const Vec2& GetWorldPos2DFromGrid(const Vec2Int& gridPos);
	const Vec3& GetWorldPosFromGrid(const Vec2Int& gridPos);


	float GetHValue(const Vec2Int& startGrid, const Vec3& destPos);
	float GetRealDistanceFromGridPos(const Vec2Int& startGrid, const Vec2Int& destGrid);
	
private:
	array<array<float, 503>, 503> heightMap;
	array<array<bool, 503>, 503> movableMap;

	


	array<array<int, 503>, 503> mobInfo;
};

extern shared_ptr<MapManager> GMapManager;

