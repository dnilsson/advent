#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day22.txt";

const int FIELD_WIDTH = 150;
const int FIELD_HEIGHT = 200;

enum Operation { Left, Right, Forward };

struct Instruction
{
	Operation op;
	int steps;		// Only for Forward
};

enum Direction { East, South, West, North };

struct Position
{
	int x;
	int y;
	Direction direction;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

int CalculatePassword(Position pos)
{
	int d = pos.direction == East ? 0
		: pos.direction == South ? 1
		: pos.direction == West ? 2
		: 3;
		
	return 1000 * (pos.y + 1) + 4 * (pos.x + 1) + d;
}

std::vector<Instruction> ParseInstructions(const std::string& s)
{
	std::vector<Instruction> result;
	
	int steps = 0;
	
	for (char c : s)
	{
		if (c == 'R')
		{
			if (steps)
				result.push_back({ Forward, steps });
			result.push_back({ Right });
			steps = 0;
		}
		else if (c == 'L')
		{
			if (steps)
				result.push_back({ Forward, steps });
			result.push_back({ Left });
			steps = 0;
		}
		else
		{
			steps = 10 * steps + static_cast<int>(c - '0');
		}
	}

	if (steps)
		result.push_back({ Forward, steps });

	return result;
}

Position GetStartPosition(char field[FIELD_WIDTH][FIELD_HEIGHT])
{
	Position position;
	position.x = 0;
	position.y = 0; 
	position.direction = East;
	while (field[position.x][position.y] != '.')
		position.x++;
	return position;
}

Position MoveForward(char field[FIELD_WIDTH][FIELD_HEIGHT], Position pos, int steps)
{
	if (steps == 0)
		return pos;
	
	Position pos2;
	pos2.direction = pos.direction;
	
	switch (pos.direction)
	{
	case North:
		pos2.x = pos.x;
		pos2.y = pos.y - 1;
		if (pos2.y < 0 || field[pos2.x][pos2.y] == ' ')
		{
			pos2.y = FIELD_HEIGHT - 1;
			while (field[pos2.x][pos2.y] == ' ')
				pos2.y--;
		}
		break;
		
	case East:
		pos2.x = pos.x + 1;
		pos2.y = pos.y;
		if (pos2.x >= FIELD_WIDTH || field[pos2.x][pos2.y] == ' ')
		{
			pos2.x = 0;
			while (field[pos2.x][pos2.y] == ' ')
				pos2.x++;
		}
		break;

	case South:
		pos2.x = pos.x;
		pos2.y = pos.y + 1;
		if (pos2.y >= FIELD_HEIGHT || field[pos2.x][pos2.y] == ' ')
		{
			pos2.y = 0;
			while (field[pos2.x][pos2.y] == ' ')
				pos2.y++;
		}
		break;

	default:
		pos2.x = pos.x - 1;
		pos2.y = pos.y;
		if (pos2.x < 0 || field[pos2.x][pos2.y] == ' ')
		{
			pos2.x = FIELD_WIDTH - 1;
			while (field[pos2.x][pos2.y] == ' ')
				pos2.x--;
		}
		break;
	}

	if (field[pos2.x][pos2.y] == '#')
		return pos;
	else
		return MoveForward(field, pos2, steps - 1);
}

Position Move(char field[FIELD_WIDTH][FIELD_HEIGHT], Position pos, Instruction instruction)
{
	Position pos2;

	switch (instruction.op)
	{
	case Left:
		pos2.x = pos.x;
		pos2.y = pos.y;
		pos2.direction = pos.direction == North ? West
			: pos.direction == West ? South
			: pos.direction == South ? East
			: North;
		break;
	case Right:
		pos2.x = pos.x;
		pos2.y = pos.y;
		pos2.direction = pos.direction == North ? East
			: pos.direction == East ? South
			: pos.direction == South ? West
			: North;
		break;
	default:
		return MoveForward(field, pos, instruction.steps);
	}

	return pos2;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);

	char field[FIELD_WIDTH][FIELD_HEIGHT];
	for (int x = 0; x < FIELD_WIDTH; x++)
		for (int y = 0; y < FIELD_HEIGHT; y++)
			field[x][y] = ' ';
			
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		const auto& line = lines[y];
		for (int x = 0; x < line.size(); x++)
			field[x][y] = line[x];
	}	
	
	auto instructions = ParseInstructions(lines[lines.size() - 1]);
	
	Position pos = GetStartPosition(field);
	for (Instruction instruction : instructions)
		pos = Move(field, pos, instruction);

	std::cout << "Result: " << CalculatePassword(pos) << "\n";
	return 0;
}
