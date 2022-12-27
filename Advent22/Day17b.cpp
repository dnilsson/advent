#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day17.txt";

const int CAVE_WIDTH = 7;
const int NUM_ROCKS = 50000000;
const int CYCLE_LENGTH_FACTOR = 50455;

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


struct RockResult
{
	int height;
	int diff;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<RockResult> GetFinalHeight(const std::string& jetDirections)
{
	std::vector<RockResult> result;

	Cave cave;
	RockGenerator rockGenerator(cave);
	JetGenerator jetGenerator(jetDirections);
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
			auto height = cave.GetRockPileHeight();
			result.push_back({ height, result.empty() ? height : height - result[result.size() - 1].height });
			if (numFinishedRocks == NUM_ROCKS)
				break;
		}
	}
	
	return result;
}

int FindCycle(const std::vector<RockResult>& stats)
{
	int cycleLength = CYCLE_LENGTH_FACTOR;
	
	for (;;)
	{
		bool isCycle = true;	
		for (int i = 0; i < cycleLength; i++)
		{
			if (stats[stats.size() - 1 - i].diff != stats[stats.size() - 1 - i - cycleLength].diff)
			{
				isCycle = false;
				break;
			}
		}
		
		if (isCycle)
			return cycleLength;
			
		cycleLength += CYCLE_LENGTH_FACTOR;
	}
}

long CalculateHeight(long numDroppedRocks, int cycleStartPosition, int heightAtCycleStart, const std::vector<int>& cycleDeltas)
{
	long entireCycleDelta = 0;
	for (int i = 0; i < cycleDeltas.size(); i++)
		entireCycleDelta += cycleDeltas[i];

	long numMinusProlog = numDroppedRocks - cycleStartPosition;
	long height = heightAtCycleStart;
	
	long numEntireCycles = numMinusProlog / cycleDeltas.size();
	height += numEntireCycles * entireCycleDelta;

	auto numInLastCycle = numMinusProlog - numEntireCycles * cycleDeltas.size();
	for (int i = 0; i < numInLastCycle; i++)
		height += cycleDeltas[i];
	
	return height;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{	
	auto lines = AdventUtil::ReadFile(FILENAME);

	auto stats = GetFinalHeight(lines[0]);
	auto cycleLength = FindCycle(stats);

	std::vector<int> cycleDeltas(cycleLength);
	for (int i = 0; i < cycleLength; i++)
		cycleDeltas[i] = stats[stats.size() - cycleLength + i].diff;
	auto finalHeight = CalculateHeight(1000000000000, stats.size() - cycleLength, stats[stats.size() - cycleLength].height, cycleDeltas);

	std::cout << "Result: " << finalHeight << "\n";
	return 0;
}
