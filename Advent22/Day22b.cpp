#include "AdventUtil.h"

#include <iostream>
#include <unordered_map>

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

struct PositionWrap
{
	Position fromPos;
	Position toPos;
};

struct Rect
{
	int x1;
	int y1;
	int x2;
	int y2;
};

struct EdgeConnection
{
	int areaId1;
	Direction direction1;
	int areaId2;
	Direction direction2;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions

int CalculatePassword(Position pos)
{
	int d = pos.direction == East ? 0
		: pos.direction == South ? 1
		: pos.direction == West ? 2
		: 3;

	return 1000 * (pos.y + 1) + 4 * (pos.x + 1) + d;
}

void CreateWraps(const Rect& sourceRect, Direction sourceDirection,
	const Rect& destinationRect, Direction destinationDirection, std::vector<PositionWrap>& wraps)
{
	int sourceX = 0;
	int sourceY = 0;
	int destinationX = 0;
	int destinationY = 0;

	for (int i = 0; i < sourceRect.x2 - sourceRect.x1 + 1; i++)
	{
		switch (sourceDirection)
		{
		case North:
			sourceY = sourceRect.y1 - 1;
			sourceX = sourceRect.x1 + i;
			break;
		case East:
			sourceY = sourceRect.y1 + i;
			sourceX = sourceRect.x2 + 1;
			break;
		case South:
			sourceY = sourceRect.y2 + 1;
			sourceX = sourceRect.x2 - i;
			break;
		case West:
			sourceY = sourceRect.y2 - i;
			sourceX = sourceRect.x1 - 1;
			break;	
		}

		Direction newDirection = North;
		switch (destinationDirection)
		{
		case North:
			destinationY = destinationRect.y1;
			destinationX = destinationRect.x2 - i;
			newDirection = South;
			break;
		case East:
			destinationY = destinationRect.y2 - i;
			destinationX = destinationRect.x2;
			newDirection = West;
			break;
		case South:
			destinationY = destinationRect.y2;
			destinationX = destinationRect.x1 + i;
			newDirection = North;
			break;
		case West:
			destinationY = destinationRect.y1 + i;
			destinationX = destinationRect.x1;
			newDirection = East;
			break;	
		}
		
 		wraps.push_back({{sourceX, sourceY, sourceDirection}, {destinationX, destinationY, newDirection}});
	}
}

std::vector<PositionWrap> CreateWraps(const std::unordered_map<int, Rect>& areas, const std::vector<EdgeConnection>& connections)
{
	std::vector<PositionWrap> wraps;

	for (const auto& connection : connections)
	{
		const auto& area1 = (*areas.find(connection.areaId1)).second;
		const auto& area2 = (*areas.find(connection.areaId2)).second;
		CreateWraps(area1, connection.direction1, area2, connection.direction2, wraps);
		CreateWraps(area2, connection.direction2, area1, connection.direction1, wraps);
	}

	return wraps;
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

Position Wrap(const std::vector<PositionWrap>& wraps, const Position& pos)
{
	for (const auto& wrap : wraps)
	{
		if (wrap.fromPos.x == pos.x && wrap.fromPos.y == pos.y && wrap.fromPos.direction == pos.direction)
			return wrap.toPos;
	}
	return pos;
}

Position MoveForward(char field[FIELD_WIDTH][FIELD_HEIGHT], const std::vector<PositionWrap>& wraps, Position pos, int steps)
{
	if (steps == 0)
		return pos;
	
	Position pos2;
	pos2.x = pos.direction == East ? pos.x + 1 : pos.direction == West ? pos.x - 1 : pos.x;
	pos2.y = pos.direction == South ? pos.y + 1 : pos.direction == North ? pos.y - 1 : pos.y;
	pos2.direction = pos.direction;
	
	pos2 = Wrap(wraps, pos2);

	if (field[pos2.x][pos2.y] == '#')
		return pos;
	else
		return MoveForward(field, wraps, pos2, steps - 1);
}

Position Move(char field[FIELD_WIDTH][FIELD_HEIGHT], const std::vector<PositionWrap>& wraps, Position pos, Instruction instruction)
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
		return MoveForward(field, wraps, pos, instruction.steps);
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
	
	auto wraps = CreateWraps(
		{
			{ 1, { 50, 0, 99, 49 }},
			{ 2, { 100, 0, 149, 49 }},
			{ 3, { 50, 50, 99, 99 }},
			{ 4, { 0, 100, 49, 149 }},
			{ 5, { 50, 100, 99, 149 }},
			{ 6, { 0, 150, 49, 199 }}
		}, {
			{ 1, West, 4, West },
			{ 3, West, 4, North },
			{ 6, East, 5, South },
			{ 3, East, 2, South },
			{ 1, North, 6, West },
			{ 2, East, 5, East },
			{ 2, North, 6, South }
		});
	
	Position pos = GetStartPosition(field);
	for (Instruction instruction : instructions)
		pos = Move(field, wraps, pos, instruction);
	
	std::cout << "Result: " << CalculatePassword(pos) << "\n";
	return 0;
}
