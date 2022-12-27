#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day23.txt";

const int BOARD_SIZE = 105;
const int MARGIN = 15;

const int NUM_ROUNDS = 10;

enum Direction { North = 0, South = 1, West = 2, East = 3 };


///////////////////////////////////////////////////////////////////////////////
// Helper functions

void InitializeBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
	auto lines = AdventUtil::ReadFile(FILENAME);

	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			board[x][y] = '.';

	for (int y = 0; y < lines.size(); y++)
	{
		const auto& line = lines[y];
		for (int x = 0; x < line.size(); x++)
		{
			char c = line[x];
			board[x + MARGIN][y + MARGIN] = c;
		}
	}
}

int CountEmptyTiles(char board[BOARD_SIZE][BOARD_SIZE])
{
	int x0 = -1;
	for (bool empty = true; empty;)
	{
		x0++;
		for (int y = 0; y < BOARD_SIZE; y++)
			if (board[x0][y] != '.')
				empty = false;
	}

	int y0 = -1;
	for (bool empty = true; empty;)
	{
		y0++;
		for (int x = 0; x < BOARD_SIZE; x++)
			if (board[x][y0] != '.')
				empty = false;
	}
	
	int x1 = BOARD_SIZE;
	for (bool empty = true; empty;)
	{
		x1--;
		for (int y = 0; y < BOARD_SIZE; y++)
			if (board[x1][y] != '.')
				empty = false;
	}
	
	int y1 = BOARD_SIZE;
	for (bool empty = true; empty;)
	{
		y1--;
		for (int x = 0; x < BOARD_SIZE; x++)
			if (board[x][y1] != '.')
				empty = false;
	}

	int emptyCount = 0;
	for (int x = x0; x <= x1; x++)
		for (int y = y0; y <= y1; y++)
			if (board[x][y] == '.')
				emptyCount++;

	return emptyCount;	
}

bool SqrEmpty(char board[BOARD_SIZE][BOARD_SIZE], int x, int y)
{
	return board[x][y] == '.' || board[x][y] == '1' || board[x][y] == '2';
}

void MarkSquare(char board[BOARD_SIZE][BOARD_SIZE], const std::vector<Direction> directions, int x, int y)
{
	if (SqrEmpty(board, x - 1, y - 1) && SqrEmpty(board, x, y - 1) && SqrEmpty(board, x + 1, y - 1) && SqrEmpty(board, x - 1, y)
		&& SqrEmpty(board, x + 1, y) && SqrEmpty(board, x - 1, y + 1) && SqrEmpty(board, x, y + 1) && SqrEmpty(board, x + 1, y + 1))
		return;

	for (Direction d : directions)
	{
		if (d == North)
		{
			if (SqrEmpty(board, x - 1, y - 1) && SqrEmpty(board, x, y - 1) && SqrEmpty(board, x + 1, y - 1))
			{
				board[x][y - 1] = (board[x][y - 1] == '.' ? '1' : '2');
				board[x][y] = 'N';
				break;
			}
		}
		else if (d == South)
		{
			if (SqrEmpty(board, x - 1, y + 1) && SqrEmpty(board, x, y + 1) && SqrEmpty(board, x + 1, y + 1))
			{
				board[x][y + 1] = (board[x][y + 1] == '.' ? '1' : '2');
				board[x][y] = 'S';
				break;
			}
		}
		else if (d == West)
		{
			if (SqrEmpty(board, x - 1, y - 1) && SqrEmpty(board, x - 1, y) && SqrEmpty(board, x - 1, y + 1))
			{
				board[x - 1][y] = (board[x - 1][y] == '.' ? '1' : '2');
				board[x][y] = 'W';
				break;
			}
		}
		else if (d == East)
		{
			if (SqrEmpty(board, x + 1, y - 1) && SqrEmpty(board, x + 1, y) && SqrEmpty(board, x + 1, y + 1))
			{
				board[x + 1][y] = (board[x + 1][y] == '.' ? '1' : '2');
				board[x][y] = 'E';
				break;
			}
		}
	}
}

bool MoveSquare(char board[BOARD_SIZE][BOARD_SIZE], int x, int y)
{
	char c = board[x][y];
	if (c != 'N' && c != 'S' && c != 'W' && c != 'E')
		return false;
		
	int dx = c == 'W' ? -1 : c == 'E' ? 1 : 0;
	int dy = c == 'N' ? -1 : c == 'S' ? 1 : 0;

	if (board[x + dx][y + dy] == '1')
	{
		board[x + dx][y + dy] = '#';
		board[x][y] = '.';
		return true;
	}
	else
	{
		board[x][y] = '#';
		return false;
	}
}

bool PlayRound(char board[BOARD_SIZE][BOARD_SIZE], const std::vector<Direction> directions)
{
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (board[x][y] == '#')
				MarkSquare(board, directions, x, y);
		
	bool movement = false;
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (MoveSquare(board, x, y))
				movement = true;
	
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (board[x][y] == '2')
				board[x][y] = '.';
				
	return movement;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	char board[BOARD_SIZE][BOARD_SIZE];
	InitializeBoard(board);
	std::vector<Direction> directions = { North, South, West, East };

	for (int i = 0; i < NUM_ROUNDS; i++)
	{
		PlayRound(board, directions);
		Direction t = directions[0];
		for (int j = 0; j < 3; j++)
			directions[j] = directions[j + 1];
		directions[3] = t;		
	}

	std::cout << "Result: " << CountEmptyTiles(board) << "\n";
	return 0;
}
