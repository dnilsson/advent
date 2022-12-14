#include "AdventUtil.h"

#include <iostream>

const int SIZE = 99;
const char* FILENAME = "Day08.txt";

//////////////////////////////////////////////////////////////////////////////////////////

class Tree
{
private:
	int _height = -1;

	Tree* _northNeighbor = nullptr;
	Tree* _southNeighbor = nullptr;
	Tree* _westNeighbor = nullptr;
	Tree* _eastNeighbor = nullptr;

	enum Visibility { Undecided, Invisible, Visible };
	
	Visibility _visibleFromNorth = Undecided;
	Visibility _visibleFromSouth = Undecided;
	Visibility _visibleFromWest = Undecided;
	Visibility _visibleFromEast = Undecided;
	
public:
	void SetHeight(int height)
	{
		_height = height;
	}

	void SetNeighbors(Tree* northNeighbor, Tree* southNeighbor, Tree* westNeighbor, Tree* eastNeighbor)
	{
		_northNeighbor = northNeighbor;
		_southNeighbor = southNeighbor;
		_westNeighbor = westNeighbor;
		_eastNeighbor = eastNeighbor;
	}

	Tree* GetNorthNeighbor() { return _northNeighbor; }
	Tree* GetSouthNeighbor() { return _southNeighbor; }
	Tree* GetWestNeighbor() { return _westNeighbor; }
	Tree* GetEastNeighbor() { return _eastNeighbor; }

	int GetHeight()
	{
		return _height;
	}

	bool IsVisibleFromNorth()
	{
		if (_visibleFromNorth == Undecided)
		{
			_visibleFromNorth = Visible;
			for (Tree* neighbor = GetNorthNeighbor(); neighbor != nullptr; neighbor = neighbor->GetNorthNeighbor())
			{
				if (neighbor->GetHeight() >= _height)
				{
					_visibleFromNorth = Invisible;
					break;
				}
			}
		}
		return _visibleFromNorth == Visible;
	}

	bool IsVisibleFromSouth()
	{
		if (_visibleFromSouth == Undecided)
		{
			_visibleFromSouth = Visible;
			for (Tree* neighbor = GetSouthNeighbor(); neighbor != nullptr; neighbor = neighbor->GetSouthNeighbor())
			{
				if (neighbor->GetHeight() >= _height)
				{
					_visibleFromSouth = Invisible;
					break;
				}
			}
		}
		return _visibleFromSouth == Visible;
	}

	bool IsVisibleFromWest()
	{
		if (_visibleFromWest == Undecided)
		{
			_visibleFromWest = Visible;
			for (Tree* neighbor = GetWestNeighbor(); neighbor != nullptr; neighbor = neighbor->GetWestNeighbor())
			{
				if (neighbor->GetHeight() >= _height)
				{
					_visibleFromWest = Invisible;
					break;
				}
			}
		}
		return _visibleFromWest == Visible;
	}

	bool IsVisibleFromEast()
	{
		if (_visibleFromEast == Undecided)
		{
			_visibleFromEast = Visible;
			for (Tree* neighbor = GetEastNeighbor(); neighbor != nullptr; neighbor = neighbor->GetEastNeighbor())
			{
				if (neighbor->GetHeight() >= _height)
				{
					_visibleFromEast = Invisible;
					break;
				}
			}
		}
		return _visibleFromEast == Visible;
	}
	
	bool IsVisible()
	{
		return IsVisibleFromNorth() || IsVisibleFromSouth() || IsVisibleFromWest() || IsVisibleFromEast();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Tree trees[SIZE][SIZE];

	auto lines = AdventUtil::ReadFile(FILENAME);

	for (int y = 0; y < SIZE; y++)
	{
		const auto& line = lines[y];
		for (int x = 0; x < SIZE; x++)
		{
			int height = line[x] - '0';
			trees[x][y].SetHeight(height);
	
			trees[x][y].SetNeighbors(
				y > 0 ? &(trees[x][y - 1]) : nullptr,
				y < (SIZE - 1) ? &(trees[x][y + 1]) : nullptr,
				x > 0 ? &(trees[x - 1][y]) : nullptr,
				x < (SIZE - 1) ? &(trees[x + 1][y]) : nullptr
				);
		}
	}

	int visibleCount = 0;
	
	for (int y = 0; y < SIZE; y++)
	{
		for (int x = 0; x < SIZE; x++)
		{
			std::cout << (trees[x][y].IsVisible() ? "1" : "0");
			if (trees[x][y].IsVisible())
				visibleCount++;
		}
		std::cout << "\n";
	}
	
	std::cout << "Result: " << visibleCount << "\n";

	return 0;
}
