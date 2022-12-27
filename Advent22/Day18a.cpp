#include "AdventUtil.h"

#include <iostream>

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
	if (space[x][y][z] == 0)
		return 0;
		
	int result = 0;
	if (x == SPACE_SIZE - 1 || space[x + 1][y][z] == 0)
		result++;
	if (x == 0 || space[x - 1][y][z] == 0)
		result++;
	if (y == SPACE_SIZE - 1 || space[x][y + 1][z] == 0)
		result++;
	if (y == 0 || space[x][y - 1][z] == 0)
		result++;
	if (z == SPACE_SIZE - 1 || space[x][y][z + 1] == 0)
		result++;
	if (z == 0 || space[x][y][z - 1] == 0)
		result++;

	return result;
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

	int area = 0;
	for (int x = 0; x < SPACE_SIZE; x++)
		for (int y = 0; y < SPACE_SIZE; y++)
			for (int z = 0; z < SPACE_SIZE; z++)
				area += CalculateSurfaceArea(space, x, y, z);	

	std::cout << "Result: " << area << "\n";
	return 0;
}
