#include "AdventUtil.h"

#include <iostream>
#include <queue>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day18.txt";

const int SPACE_SIZE = 20;

struct Position
{
	int x;
	int y;
	int z;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

int CalculateSurfaceArea(int space[SPACE_SIZE][SPACE_SIZE][SPACE_SIZE], int x, int y, int z)
{
	if (space[x][y][z] != 1)
		return 0;
		
	int result = 0;
	if (x == SPACE_SIZE - 1 || space[x + 1][y][z] == 2)
		result++;
	if (x == 0 || space[x - 1][y][z] == 2)
		result++;
	if (y == SPACE_SIZE - 1 || space[x][y + 1][z] == 2)
		result++;
	if (y == 0 || space[x][y - 1][z] == 2)
		result++;
	if (z == SPACE_SIZE - 1 || space[x][y][z + 1] == 2)
		result++;
	if (z == 0 || space[x][y][z - 1] == 2)
		result++;

	return result;
}

void FloodFillExterior(int space[SPACE_SIZE][SPACE_SIZE][SPACE_SIZE])
{
	std::queue<Position> toFill;
	toFill.push({ 0, 0, 0 });

	int filled = 0;
	while (!toFill.empty())
	{
		Position pos = toFill.front();
		toFill.pop();
		
		if (space[pos.x][pos.y][pos.z] != 0)
			continue;
		
		space[pos.x][pos.y][pos.z] = 2;
		filled++;
		
		
		if (pos.x > 0 && space[pos.x - 1][pos.y][pos.z] == 0)
			toFill.push({ pos.x - 1, pos.y, pos.z });
		if (pos.x < SPACE_SIZE - 1 && space[pos.x + 1][pos.y][pos.z] == 0)
			toFill.push({ pos.x + 1, pos.y, pos.z });
		
		
		if (pos.y > 0 && space[pos.x][pos.y - 1][pos.z] == 0)
			toFill.push({ pos.x, pos.y - 1, pos.z });
		if (pos.y < SPACE_SIZE - 1 && space[pos.x][pos.y + 1][pos.z] == 0)
			toFill.push({ pos.x, pos.y + 1, pos.z });

		if (pos.z > 0 && space[pos.x][pos.y][pos.z - 1] == 0)
			toFill.push({ pos.x, pos.y, pos.z - 1 });
		if (pos.z < SPACE_SIZE - 1 && space[pos.x][pos.y][pos.z + 1] == 0)
			toFill.push({ pos.x, pos.y, pos.z + 1 });
	}
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{	
	auto lines = AdventUtil::ReadFile(FILENAME);

	int space[SPACE_SIZE][SPACE_SIZE][SPACE_SIZE];

	for (int x = 0; x < SPACE_SIZE; x++)
		for (int y = 0; y < SPACE_SIZE; y++)
			for (int z = 0; z < SPACE_SIZE; z++)
				space[x][y][z] = 0;
				
	for (const auto& line : lines)
	{
		auto toks = AdventUtil::SplitString(line, ',');
		int x = std::atol(toks[0].c_str());
		int y = std::atol(toks[1].c_str());
		int z = std::atol(toks[2].c_str());
		space[x][y][z] = 1;		
	}

	FloodFillExterior(space);

	int area = 0;
	for (int x = 0; x < SPACE_SIZE; x++)
		for (int y = 0; y < SPACE_SIZE; y++)
			for (int z = 0; z < SPACE_SIZE; z++)
				area += CalculateSurfaceArea(space, x, y, z);	

	std::cout << "Result: " << area << "\n";
	return 0;
}
