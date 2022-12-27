#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day17.txt";

const int CAVE_WIDTH = 7;
const int NUM_ROCKS = 2022;

class Cave
{
private:
	std::vector<std::string> _layers;	// 0 = closest to the ground

public:
	int GetRockPileHeight()
	{
		return _layers.size();
	}

	void PrintPattern(const std::vector<std::string>& pattern, int x, int y)
	{
		for (int py = 0; py < pattern.size(); py++)
		{
			while (_layers.size() < y + py + 1)
				_layers.push_back(".......");

			for (int px = 0; px < pattern[0].size(); px++)
			{
				if (pattern[py][px] == '.')
					continue;
				_layers[y + py][x + px] = pattern[py][px];
			}
		}
	}
	
	bool PatternCollides(const std::vector<std::string>& pattern, int x, int y)
	{
		for (int py = 0; py < pattern.size(); py++)
		{
			if (_layers.size() < y + py + 1)
				continue;
			
			for (int px = 0; px < pattern[0].size(); px++)
			{
				if (pattern[py][px] != '.' && _layers[y + py][x + px] != '.')
					return true;
			}
		}
		return false;
	}
};


class Rock
{
private:
	Cave& _cave;
	std::vector<std::string> _pattern;
	int _x;	// 0 = adjacent to left edge
	int _y;	// 0 = adjacent to ground

public:
	Rock(Cave& cave, const std::vector<std::string>& pattern)
		: _cave(cave)
		, _pattern(pattern)
		, _x(2)
		, _y(cave.GetRockPileHeight() + 3)
	{
	}

	bool TryDrop()
	{
		if (_y == 0 || _cave.PatternCollides(_pattern, _x, _y - 1))
			return false;
	
		_y--;
		return true;
	}
	
	void TryMoveHorizontally(int direction)
	{
		if (direction == -1)
		{
			if (_x > 0 && !_cave.PatternCollides(_pattern, _x - 1, _y))
				_x--;
		}
		else
		{
			if (_x + _pattern[0].size() < CAVE_WIDTH && !_cave.PatternCollides(_pattern, _x + 1, _y))
				_x++;
		}
	}
	
	void Print()
	{
		_cave.PrintPattern(_pattern, _x, _y);
	}
};


class RockGenerator
{
private:
	Cave& _cave;
	std::vector<std::vector<std::string>> _patterns =
	{
		{
			"AAAA"
		},
		{
			".B.",
			"BBB",
			".B."
		},
		{
			"CCC",	// Lowest row first
			"..C",
			"..C"
		},
		{
			"D",
			"D",
			"D",
			"D"
		},
		{
			"EE",
			"EE"
		}
	};
	int _position = 0;

public:
	RockGenerator(Cave& cave)
		: _cave(cave)
	{
	}

	std::shared_ptr<Rock> CreateRock()
	{
		const std::vector<std::string>& pattern = _patterns[_position];
		_position = (_position + 1) % _patterns.size();
		return std::make_shared<Rock>(_cave, pattern);
	}
};


class JetGenerator
{
private:
	std::vector<int> _directions;
	int _position = 0;

public:
	JetGenerator(const std::string& pattern)
		: _directions(pattern.size())
	{
		for (int i = 0; i < pattern.size(); i++)
			_directions[i] = (pattern[i] == '<' ? -1 : +1);
	}
	
	int GetNextJet()
	{
		int direction = _directions[_position];
		_position = (_position + 1) % _directions.size();
		return direction;
	}
};


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{	
	auto lines = AdventUtil::ReadFile(FILENAME);

	Cave cave;
	RockGenerator rockGenerator(cave);
	JetGenerator jetGenerator(lines[0]);
	std::shared_ptr<Rock> currentRock = nullptr;
	int numFinishedRocks = 0;

	for (;;)
	{
		if (!currentRock)
			currentRock = rockGenerator.CreateRock();
		int jet = jetGenerator.GetNextJet();
		currentRock->TryMoveHorizontally(jet);

		if (!currentRock->TryDrop())
		{
			currentRock->Print();
			currentRock = nullptr;
			numFinishedRocks++;
			if (numFinishedRocks == NUM_ROCKS)
				break;
		}
	}
	
	std::cout << "Result: " << cave.GetRockPileHeight() << "\n";
	return 0;
}
