#include "AdventUtil.h"

#include <iostream>
#include <queue>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day12.txt";

struct Position
{
	int x;
	int y;
};

const int MAP_WIDTH = 181;
const int MAP_HEIGHT = 41;

const Position START_POSITION = { 0, 20 };
const Position END_POSITION = { 158, 20 };


///////////////////////////////////////////////////////////////////////////////
// Helper functions

void ParseLines(const std::vector<std::string>& lines, int heights[MAP_WIDTH][MAP_HEIGHT])
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		auto& line = lines[y];
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			char c = line[x];
			heights[x][y] = c == 'S' ? 0
				: c == 'E' ? (int)('z' - 'a')
				: (int)(c - 'a');
		}
	}	
}

bool CanGoUp(int heights[MAP_WIDTH][MAP_HEIGHT], int distances[MAP_WIDTH][MAP_HEIGHT], const Position& position) 
{
	return position.y > 0 && distances[position.x][position.y - 1] == -1
		&& heights[position.x][position.y - 1] <= heights[position.x][position.y] + 1;
}

bool CanGoDown(int heights[MAP_WIDTH][MAP_HEIGHT], int distances[MAP_WIDTH][MAP_HEIGHT], const Position& position)
{
	return position.y < MAP_HEIGHT - 1 && distances[position.x][position.y + 1] == -1
		&& heights[position.x][position.y + 1] <= heights[position.x][position.y] + 1;
}

bool CanGoLeft(int heights[MAP_WIDTH][MAP_HEIGHT], int distances[MAP_WIDTH][MAP_HEIGHT], const Position& position)
{
	return position.x > 0 && distances[position.x - 1][position.y] == -1
		&& heights[position.x - 1][position.y] <= heights[position.x][position.y] + 1;
}

bool CanGoRight(int heights[MAP_WIDTH][MAP_HEIGHT], int distances[MAP_WIDTH][MAP_HEIGHT], const Position& position)
{
	return position.x < MAP_WIDTH - 1 && distances[position.x + 1][position.y] == -1
		&& heights[position.x + 1][position.y] <= heights[position.x][position.y] + 1;
}

Position GoUp(const Position& position) { return { position.x, position.y - 1 }; }
Position GoDown(const Position& position) { return { position.x, position.y + 1 }; }
Position GoLeft(const Position& position) { return { position.x - 1, position.y }; }
Position GoRight(const Position& position) { return { position.x + 1, position.y }; }


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);

	int heights[MAP_WIDTH][MAP_HEIGHT];
	ParseLines(lines, heights);	
	
	int distances[MAP_WIDTH][MAP_HEIGHT];
	for (int y = 0; y < MAP_HEIGHT; y++)
		for (int x = 0; x < MAP_WIDTH; x++)
			distances[x][y] = -1;
	distances[START_POSITION.x][START_POSITION.y] = 0;
	
	std::queue<Position> positionsToProcess;
	positionsToProcess.push(START_POSITION);

	while (!positionsToProcess.empty())
	{
		auto position = positionsToProcess.front();
		positionsToProcess.pop();
		int height = heights[position.x][position.y];
		int distance = distances[position.x][position.y];

		if (CanGoUp(heights, distances, position))
		{
			auto newPosition = GoUp(position);
			distances[newPosition.x][newPosition.y] = distance + 1;
			positionsToProcess.push(newPosition);
		}
		
		if (CanGoDown(heights, distances, position))
		{
			auto newPosition = GoDown(position);
			distances[newPosition.x][newPosition.y] = distance + 1;
			positionsToProcess.push(newPosition);
		}
		
		if (CanGoLeft(heights, distances, position))
		{
			auto newPosition = GoLeft(position);
			distances[newPosition.x][newPosition.y] = distance + 1;
			positionsToProcess.push(newPosition);
		}
		
		if (CanGoRight(heights, distances, position))
		{
			auto newPosition = GoRight(position);
			distances[newPosition.x][newPosition.y] = distance + 1;
			positionsToProcess.push(newPosition);
		}
		
	}	

	std::cout << "Result: " << distances[END_POSITION.x][END_POSITION.y] << "\n";
}
