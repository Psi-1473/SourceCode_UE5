#include "pch.h"
#include "MapManager.h"
#include <fstream>
#include <string>
#include <algorithm>

shared_ptr<MapManager> GMapManager = make_shared<MapManager>();

array<Vec2Int, 8> Direction
{
	Vec2Int(1, 1),//
	Vec2Int(1, -1),
	Vec2Int(-1, 1),
	Vec2Int(-1, -1),
	Vec2Int(0, 1),
	Vec2Int(0, -1),
	Vec2Int(1, 0),
	Vec2Int(-1, 0)
};

void MapManager::OccupyGrid(const Vec3& pos)
{
	Vec2Int gridPos = GetGridPosFromObjPos(pos);
	//cout << "Occupy : " << gridPos.y << ", " << gridPos.x << " --- " << mobInfo[gridPos.y][gridPos.x] << endl;
	mobInfo[gridPos.y][gridPos.x] = true;
}

void MapManager::ReleaseGrid(const Vec3& pos)
{
	Vec2Int gridPos = GetGridPosFromObjPos(pos);
	mobInfo[gridPos.y][gridPos.x] = false;
}

void MapManager::LoadHeightMap()
{
	string fileName = "../MapFiles/ZCoordinates.txt";
	ifstream inputFile(fileName);

	if (!inputFile.is_open()) {
		std::cerr << "파일을 열 수 없습니다: " << fileName << std::endl;
		return;

	}
	string line;

	int y = 0;
	int x = 0;
	while (getline(inputFile, line))
	{
		string segment = line;
		float height = stof(segment);
		heightMap[y][x] = height;

		x++;
		if (x == 503)
		{
			y++;
			x = 0;
		}

		if(y == 503)
			break;
	}

	inputFile.close();

	cout << heightMap[502][502] << std::endl;
}



void MapManager::LoadMovableMap()
{
	string fileName = "../MapFiles/MoveInfo.txt";
	ifstream inputFile(fileName);

	if (!inputFile.is_open()) {
		std::cerr << "파일을 열 수 없습니다: " << fileName << std::endl;
		return;

	}
	string line;

	int y = 0;
	while (getline(inputFile, line))
	{
		if (y == 0)
		{
			y++;
			continue;
		}

		if (y == 503)
			break;

		for (size_t x = 1; x < line.length(); x++)
		{
			if(x == 503)
				break;
			string segment = line.substr(x, 1);
			bool movable = (segment == "1") ? false : true;
			
			movableMap[y][x - 1] = movable;
		}
		y++;
	}

	inputFile.close();
}

void MapManager::InitializeMobInfo()
{
	for (int i = 0; i < 503; i++)
	{
		for (int j = 0; j < 503; j++)
		{
			mobInfo[i][j] = false;
		}
	}
}

vector<Vec3> MapManager::FindPath(Vec3 startPos, Vec3 destPos)
{
	// 1. StartPos와 DestPos의 그리드 인덱스를 구한다.
	// 2. Start Grid ~ Dest Index까지의 가장 가까운 경로를 찾으려고 한다.
	// 3. 그 과정에서 heightMap의 높이 좌표가 휴리스틱 값에 영향을 끼치도록 하고 movableMap의 true / false 값을 이용해
	//    갈 수 없는 그리드는 애초에 배제한다.
	const int maxInt = 2147483600;
	int maxDepth = 30;
	priority_queue<NodeInfo, std::vector<NodeInfo>, CompareNode> pq;

	// 지금까지 제일 좋은 후보 기록
	unordered_map<Vec2Int, int> best;

	// 경로 추적 용도
	unordered_map<Vec2Int, Vec2Int> parent;

	// (1) 그리드의 인덱스 좌표 구하기
	Vec2Int startGrid = GetGridPosFromObjPos(startPos);
	Vec2Int closestGrid = startGrid;
	Vec2Int destGrid = GetGridPosFromObjPos(destPos);

	// (2) 시작점을 priority queue에 넣기
	float closestH = GetHValue(closestGrid, destPos);
	pq.push(NodeInfo(closestGrid, 0, closestH, 1));

	while (!pq.empty())
	{
		NodeInfo node = pq.top();
		pq.pop();

		Vec2Int gridPos = node.gridPos;

		// 목적지 도착이면 종료
		if(gridPos == destGrid)
			break;

		// 너무 깊이 들어가진 않음(보류)
		if(node.depth >= maxDepth)
			break;

		// 8방향 확인 후, 이동할 수 있는 좌표인지 확인한다.

		for (auto dir : Direction)
		{
			Vec2Int nextGrid = gridPos + dir;
			
			if (nextGrid.x > 502 || nextGrid.x < 0 || nextGrid.y > 502 || nextGrid.y < 0)
				continue;
			if (isMovablePlace(nextGrid) == false)
				continue;
			
			float h = GetHValue(nextGrid, destPos);
			float g = node.G + GetRealDistanceFromGridPos(gridPos, nextGrid);
			float f = g + h;
			if(best.contains(nextGrid) == false)
				best.insert(make_pair(nextGrid, maxInt));

			if(best[nextGrid] <= h)
				continue;

			best[nextGrid] = h;

			pq.push(NodeInfo(nextGrid, g, h, node.depth + 1));
			parent[nextGrid] = gridPos;

			if (closestH > h)
			{
				closestH = h;
				closestGrid = nextGrid;
			}

		}
	}

	// 경로를 찾지 못했다면 대충 가까운 위치로 가게 한다.
	if(parent.contains(destGrid) == false)
		return CalcPathFromParent(parent, closestGrid, startGrid);


	return CalcPathFromParent(parent, destGrid, startGrid);
}

vector<Vec3> MapManager::CalcPathFromParent(unordered_map<Vec2Int, Vec2Int> parent, Vec2Int dest, Vec2Int start)
{
	vector<Vec3> posList;

	if(parent.contains(dest) == false)
	{
		//std::cout << "Parent Dest X" << std::endl;
		return posList;
	}

	Vec2Int now = dest;

	while (now != start)
	{
		//if(now == dest)
		//{
		//now = parent[now];
		//	continue;
		//}
		Vec3 pos = GetWorldPosFromGrid(now);
		posList.push_back(pos);
		//std::cout << "Grid(" << now.x << ", " << now.y << ") -> Grid(" << parent[now].x << ", " << parent[now].y << ")" << endl;
		now = parent[now];
	}

	posList.push_back(GetWorldPosFromGrid(now));
	std::reverse(posList.begin(), posList.end());
	//std::cout << "Calc Path From Parent" << std::endl;
	return posList;
}

bool MapManager::isMovablePlace(const Vec2Int& gridPos)
{
	//cout << "Grid Pos : " << gridPos.x << ", " << gridPos.y << endl;
	if(mobInfo[gridPos.y][gridPos.x] == true) return false;
	return (movableMap[gridPos.y][gridPos.x] == true);
}

const Vec2Int& MapManager::GetGridPosFromObjPos(const Vec3& objPos)
{
	int gridX = static_cast<int>(((25150) + objPos.x) / 100);
	int gridY = static_cast<int>(((25150) + objPos.y) / 100);

	//cout << " Grid Info -> " << gridX << ", " << gridY << endl;
	return Vec2Int(gridX, gridY);
}

const Vec2& MapManager::GetWorldPos2DFromGrid(const Vec2Int& gridPos)
{
	float x = (gridPos.x * 100) - 25100;
	float y = (gridPos.y * 100) - 25100;

	return Vec2(x, y);
}

const Vec3& MapManager::GetWorldPosFromGrid(const Vec2Int& gridPos)
{
	float z = heightMap[gridPos.y][gridPos.x];
	Vec2 planePos = GetWorldPos2DFromGrid(gridPos);
	Vec3 posInfo(planePos.x, planePos.y, z);

	return posInfo;
}

float MapManager::GetHValue(const Vec2Int& startGrid, const Vec3& destPos)
{
	//float z = heightMap[startGrid.y][startGrid.x];
	//Vec2 planePos = GetWorldPos2DFromGrid(startGrid);
	Vec3 posInfo = GetWorldPosFromGrid(startGrid);
	float heuristic = posInfo.distanceTo(destPos);

	return heuristic;
}

float MapManager::GetRealDistanceFromGridPos(const Vec2Int& startGrid, const Vec2Int& destGrid)
{
	float z = heightMap[destGrid.y][destGrid.x];
	Vec2 destPlanePos = GetWorldPos2DFromGrid(destGrid);
	Vec3 destPos(destPlanePos.x, destPlanePos.y, z);
	
	return GetHValue(startGrid, destPos);
}


