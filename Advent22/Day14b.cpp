#include "AdventUtil.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day14.txt";

const int BOARD_HEIGHT = 166 + 2;	// Max y coord from file + 1 + 2 extra to make room for the floor.
const int BOARD_WIDTH = 1000;		// Should be safe, with some margin. Rocks should keep within 500-168 and 600+168.

const int START_X = 500;
const int START_Y = 0;

struct Coords
{
	int x;
	int y;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<Coords> ParseLine(const std::string& line)
{
	std::vector<Coords> result;
	auto tokens = AdventUtil::SplitString(line);
	
	for (const std::string& token : tokens)
	{
		if (token != "->")
		{
			auto numbers = AdventUtil::SplitString(token, ',');
			result.push_back({ (int)std::atol(numbers[0].c_str()), (int)std::atol(numbers[1].c_str()) });
		}
	}
	
	return result;
}

void DrawMultiLine(char board[BOARD_WIDTH][BOARD_HEIGHT], const std::vector<Coords>& coordPairs)
{
	int x = coordPairs[0].x;
	int y = coordPairs[0].y;
	board[x][y] = '=';
	
	for (int i = 1; i < coordPairs.size(); i++)
	{
		Coords nextCoords = coordPairs[i];
		while (x != nextCoords.x || y != nextCoords.y)
		{
			int dx = nextCoords.x - x;
			int dy = nextCoords.y - y;
		
			x += dx > 0 ? 1 : dx < 0 ? -1 : 0;
			y += dy > 0 ? 1 : dy < 0 ? -1 : 0;
		
			board[x][y] = '=';
		}
	}
}

bool DropRock(char board[BOARD_WIDTH][BOARD_HEIGHT])
{
	int x = START_X;
	int y = START_Y;
	
	if (board[x][y] != ' ')
		return false;
		
	while (y < BOARD_HEIGHT)
	{
		if (board[x][y + 1] == ' ')
		{
			y++;
		}
		else if (board[x - 1][y + 1] == ' ')
		{
			x--;
			y++;
		}
		else if (board[x + 1][y + 1] == ' ')
		{
			x++;
			y++;
		}
		else
		{
			board[x][y] = '#';
			return true;
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	char board[BOARD_WIDTH][BOARD_HEIGHT];
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			board[x][y] = ' ';

	auto lines = AdventUtil::ReadFile(FILENAME);

	std::vector<std::vector<Coords>> multiLines;	
	for (const auto& line : lines)
	{
		auto coordPairs = ParseLine(line);
		multiLines.push_back(coordPairs);
	}

	for (const auto& multiLine : multiLines)
	{
		DrawMultiLine(board, multiLine);
	}

	for (int x = 0; x < BOARD_WIDTH; x++)
		board[x][BOARD_HEIGHT - 1] = '=';

	int rocksPlaced = 0;
	while (DropRock(board))
		rocksPlaced++;

	std::cout << "Rocks placed: " << rocksPlaced << "\n";	
	return 0;
}
