#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day24.txt";

const int BOARD_WIDTH = 122;
const int BOARD_HEIGHT = 27;

const int START_X = 1;
const int START_Y = 0;
const int TARGET_X = 120;
const int TARGET_Y = 26;

enum Direction { North, South, East, West };

struct Blizzard
{
	int xpos;
	int ypos;
	Direction dir;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<Blizzard> ParseBlizzards(const std::vector<std::string>& lines)
{
	std::vector<Blizzard> result;
	
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		const auto& line = lines[y];
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			char c = line[x];
			
			if (c == '>')
				result.push_back({ x, y, East });
			else if (c == '<')
				result.push_back({ x, y, West });
			else if (c == 'v')
				result.push_back({ x, y, South });
			else if (c == '^')
				result.push_back({ x, y, North });
		}
	}

	return result;
}

void RenderBoard(char board[BOARD_WIDTH][BOARD_HEIGHT], const std::vector<Blizzard> blizzards)
{
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
			board[x][y] = '.';

	for (int x = 0; x < BOARD_WIDTH - 2; x++)
	{
		board[x][BOARD_HEIGHT - 1] = '#';
		board[x + 2][0] = '#';
	}
	
	for (int y = 0; y < BOARD_HEIGHT - 1; y++)
	{
		board[0][y] = '#';
		board[BOARD_WIDTH - 1][y + 1] = '#';
	}
	
	for (const auto& blizzard : blizzards)
		board[blizzard.xpos][blizzard.ypos] = 'O';
}

void StepBlizzards(std::vector<Blizzard>& blizzards)
{
	for (auto& blizzard : blizzards)
	{
		if (blizzard.dir == North)
		{
			blizzard.ypos--;
			if (blizzard.ypos == 0)
				blizzard.ypos = BOARD_HEIGHT - 2;
		}
		else if (blizzard.dir == South)
		{
			blizzard.ypos++;
			if (blizzard.ypos == BOARD_HEIGHT - 1)
				blizzard.ypos = 1;
		}
		else if (blizzard.dir == East)
		{
			blizzard.xpos++;
			if (blizzard.xpos == BOARD_WIDTH - 1)
				blizzard.xpos = 1;
		}
		else if (blizzard.dir == West)
		{
			blizzard.xpos--;
			if (blizzard.xpos == 0)
				blizzard.xpos = BOARD_WIDTH - 2;
		}
	}
}

void FillPossiblePositions(char prev[BOARD_WIDTH][BOARD_HEIGHT], char board[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int x = 1; x < BOARD_WIDTH - 1; x++)
	{
		for (int y = 1; y < BOARD_HEIGHT - 1; y++)
		{
			if (board[x][y] == '.' && (prev[x][y] == 'E' || prev[x - 1][y] == 'E'
					|| prev[x + 1][y] == 'E' || prev[x][y - 1] == 'E' || prev[x][y + 1] == 'E'))
				board[x][y] = 'E';
		}
	}

	if (prev[START_X][START_Y + 1] == 'E' || prev[START_X][START_Y] == 'E')
		board[START_X][START_Y] = 'E';
	
	if (prev[TARGET_X][TARGET_Y - 1] == 'E' || prev[TARGET_X][TARGET_Y] == 'E')
		board[TARGET_X][TARGET_Y] = 'E';
}

void CopyBoard(char source[BOARD_WIDTH][BOARD_HEIGHT], char dest[BOARD_WIDTH][BOARD_HEIGHT])
{
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
			dest[x][y] = source[x][y];
}

int SolveBoard(char board[BOARD_WIDTH][BOARD_HEIGHT], std::vector<Blizzard>& blizzards, int targetX, int targetY)
{
	int turns = 0;

	do
	{
		char prev[BOARD_WIDTH][BOARD_HEIGHT];
		CopyBoard(board, prev);
		StepBlizzards(blizzards);
		RenderBoard(board, blizzards);
		FillPossiblePositions(prev, board);
		turns++;
	}
	while (board[targetX][targetY] != 'E');
	
	return turns;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto blizzards = ParseBlizzards(lines);

	int turns = 0;
	char board[BOARD_WIDTH][BOARD_HEIGHT];
	RenderBoard(board, blizzards);
	board[START_X][START_Y] = 'E';
	turns += SolveBoard(board, blizzards, TARGET_X, TARGET_Y);

	RenderBoard(board, blizzards);
	board[TARGET_X][TARGET_Y] = 'E';
	turns += SolveBoard(board, blizzards, START_X, START_Y);
	
	RenderBoard(board, blizzards);
	board[START_X][START_Y] = 'E';
	turns += SolveBoard(board, blizzards, TARGET_X, TARGET_Y);

	std::cout << "Result: " << turns << "\n";
	return 0;
}
