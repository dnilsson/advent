#include "AdventUtil.h"

#include <iostream>

const std::string FILENAME = "Day08.txt";

const int FOREST_SIZE = 99;

//////////////////////////////////////////////////////////////////////////////////////////

class Tree
{
private:
	int _height = -1;

	Tree* _northNeighbor = nullptr;
	Tree* _southNeighbor = nullptr;
	Tree* _westNeighbor = nullptr;
	Tree* _eastNeighbor = nullptr;

public:
	void SetHeight(int height)
	{
		_height = height;
	}

	void SetNeighbors(Tree* northNeighbor, Tree* southNeighbor, Tree* westNeighbor, Tree* eastNeighbor)
	{
		_northNeighbor = northNeighbor;
		_southNeighbor = southNeighbor;
		_westNeighbor = westNeighbor;
		_eastNeighbor = eastNeighbor;
	}

	Tree* GetNorthNeighbor() { return _northNeighbor; }
	Tree* GetSouthNeighbor() { return _southNeighbor; }
	Tree* GetWestNeighbor() { return _westNeighbor; }
	Tree* GetEastNeighbor() { return _eastNeighbor; }

	int GetHeight()
	{
		return _height;
	}

	int GetViewingDistanceNorth()
	{
		int distance = 0;
		for (Tree* neighbor = GetNorthNeighbor(); neighbor != nullptr; neighbor = neighbor->GetNorthNeighbor())
		{
			distance++;
			if (neighbor->GetHeight() >= _height)
				break;
		}	
		return distance;
	}
	
	int GetViewingDistanceSouth()
	{
		int distance = 0;
		for (Tree* neighbor = GetSouthNeighbor(); neighbor != nullptr; neighbor = neighbor->GetSouthNeighbor())
		{
			distance++;
			if (neighbor->GetHeight() >= _height)
				break;
		}	
		return distance;
	}
	
	int GetViewingDistanceWest()
	{
		int distance = 0;
		for (Tree* neighbor = GetWestNeighbor(); neighbor != nullptr; neighbor = neighbor->GetWestNeighbor())
		{
			distance++;
			if (neighbor->GetHeight() >= _height)
				break;
		}	
		return distance;
	}
	
	int GetViewingDistanceEast()
	{
		int distance = 0;
		for (Tree* neighbor = GetEastNeighbor(); neighbor != nullptr; neighbor = neighbor->GetEastNeighbor())
		{
			distance++;
			if (neighbor->GetHeight() >= _height)
				break;
		}	
		return distance;
	}

	int GetScenicScore()
	{
		return GetViewingDistanceNorth() * GetViewingDistanceSouth() * GetViewingDistanceWest() * GetViewingDistanceEast();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Tree trees[FOREST_SIZE][FOREST_SIZE];

	auto lines = AdventUtil::ReadFile(FILENAME);

	for (int y = 0; y < FOREST_SIZE; y++)
	{
		const auto& line = lines[y];
		for (int x = 0; x < FOREST_SIZE; x++)
		{
			int height = line[x] - '0';
			trees[x][y].SetHeight(height);
	
			trees[x][y].SetNeighbors(
				y > 0 ? &(trees[x][y - 1]) : nullptr,
				y < (FOREST_SIZE - 1) ? &(trees[x][y + 1]) : nullptr,
				x > 0 ? &(trees[x - 1][y]) : nullptr,
				x < (FOREST_SIZE - 1) ? &(trees[x + 1][y]) : nullptr
				);
		}
	}

	int maxScenicScore = 0;
	
	for (int y = 0; y < FOREST_SIZE; y++)
	{
		for (int x = 0; x < FOREST_SIZE; x++)
		{
			int scenicScore = trees[x][y].GetScenicScore();
			if (scenicScore > maxScenicScore)
				maxScenicScore = scenicScore;
		}
	}
	
	std::cout << "Result: " << maxScenicScore << "\n";
	return 0;
}
