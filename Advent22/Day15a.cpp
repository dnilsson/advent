#include "AdventUtil.h"

#include <iostream>
#include <unordered_set>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day15.txt";

const int MEASURE_Y_POS = 2000000;

struct Position
{
	int x;
	int y;
};

class Interval
{
private:
	int _leftEdge;
	int _rightEdge;
	
public:
	Interval(int leftEdge, int rightEdge)
		: _leftEdge(leftEdge)
		, _rightEdge(rightEdge)
	{
	}

	bool Overlaps(Interval other) const
	{
		return (_leftEdge <= other._leftEdge && _rightEdge >= other._leftEdge)
			|| (other._leftEdge <= _leftEdge && other._rightEdge >= _leftEdge);
	}
	
	void Merge(Interval other)
	{
		if (other._leftEdge < _leftEdge)
			_leftEdge = other._leftEdge;
		if (other._rightEdge > _rightEdge)
			_rightEdge = other._rightEdge;
	}
	
	int GetWidth() const
	{
		return _rightEdge - _leftEdge + 1;
	}
	
	int GetLeftEdge() const
	{
		return _leftEdge;
	}
	
	int GetRightEdge() const
	{
		return _rightEdge;
	}
};

class Diamond
{
private:
	int _centerX;
	int _centerY;
	int _manhattanRadius;
	
public:
	Diamond(int centerX, int centerY, int edgePointX, int edgePointY)
		: _centerX(centerX)
		, _centerY(centerY)
		, _manhattanRadius(std::abs(edgePointX - centerX) + std::abs(edgePointY - centerY))
	{
	}
	
	Interval GetIntervalAtVerticalPosition(int yPosition) const
	{
		int radius = _manhattanRadius - std::abs(yPosition - _centerY);
		if (radius < 0)
			return { 0, -1 };
		return { _centerX - radius, _centerX + radius };
	}
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

void ParseLines(const std::vector<std::string>& lines, std::vector<Diamond>& diamonds, std::vector<Position>& beaconPositions)
{
	diamonds.clear();
	beaconPositions.clear();
	
	for (const auto& line : lines)
	{
		auto tokens = AdventUtil::SplitString(line);

		int centerX = atol(tokens[2].substr(2, tokens[2].size() - 3).c_str());
		int centerY = atol(tokens[3].substr(2, tokens[3].size() - 3).c_str());
		int edgePointX = atol(tokens[8].substr(2, tokens[8].size() - 3).c_str());
		int edgePointY = atol(tokens[9].substr(2, tokens[9].size() - 2).c_str());

		diamonds.push_back({ centerX, centerY, edgePointX, edgePointY });
		beaconPositions.push_back({ edgePointX, edgePointY });
	}
}

void ConsolidateIntervals(std::vector<Interval>& intervals)
{
	std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b){ return a.GetLeftEdge() < b.GetLeftEdge(); });
	
	int n = 0;
	for (;;)
	{
		if (n >= intervals.size() - 1)
		{
			return;
		}
		else if (intervals[n].Overlaps(intervals[n + 1]))
		{
			intervals[n].Merge(intervals[n + 1]);
			intervals.erase(intervals.begin() + (n + 1));
		}
		else
		{
			n++;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	std::vector<Diamond> diamonds;
	std::vector<Position> beaconPositions;
	ParseLines(lines, diamonds, beaconPositions);
	
	std::vector<Interval> intervals;
	for (auto diamond : diamonds)
	{
		auto interval = diamond.GetIntervalAtVerticalPosition(MEASURE_Y_POS);
		if (interval.GetWidth () >= 1)
			intervals.push_back({ diamond.GetIntervalAtVerticalPosition(MEASURE_Y_POS) });
	}

	ConsolidateIntervals(intervals);

	std::unordered_set<int> beaconXPositionsAtLine;
	for (auto position : beaconPositions)
	{
		if (position.y == MEASURE_Y_POS)
			beaconXPositionsAtLine.insert(position.x);
	}

	int totalWidth = 0;
	for (auto interval : intervals)
	{
		totalWidth += interval.GetWidth();
		for (auto x: beaconXPositionsAtLine)
		{
			if (x >= interval.GetLeftEdge() && x <= interval.GetRightEdge())
				totalWidth--;
		}
	}

	std::cout << "Result: " << totalWidth << "\n";
	return 0;
}
