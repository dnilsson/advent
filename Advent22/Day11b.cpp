#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Constants and types

const int NUM_ROUNDS = 10000;

class Item
{
private:
	std::vector<int> _divisors;
	std::vector<int> _modulos;
	
public:
	Item(const std::vector<int>& divisors, int startValue)
		: _divisors(divisors)
	{
		for (int divisor : _divisors)
			_modulos.push_back(startValue % divisor);
	}
	
	void Add(int value)
	{
		for (int i = 0; i < _divisors.size(); i++)
			_modulos[i] = (_modulos[i] + value) % _divisors[i];
	}
	
	void Multiply(int value)
	{
		for (int i = 0; i < _divisors.size(); i++)
			_modulos[i] = (_modulos[i] * value) % _divisors[i];
	}
	
	void Square()
	{
		for (int i = 0; i < _divisors.size(); i++)
			_modulos[i] = (_modulos[i] * _modulos[i]) % _divisors[i];
	}

	bool IsDivisible(int divisorIndex)
	{
		return _modulos[divisorIndex] == 0;
	}
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<std::vector<Item>> InitializeItems()
{
	std::vector<int> divisors = { 11, 3, 5, 7, 19, 2, 13, 17 };	
	std::vector<std::vector<Item>> items;

	items.push_back({
		Item(divisors, 56),
		Item(divisors, 52),
		Item(divisors, 58),
		Item(divisors, 96),
		Item(divisors, 70),
		Item(divisors, 75),
		Item(divisors, 72)
		});
	items.push_back({
		Item(divisors, 75),
		Item(divisors, 58),
		Item(divisors, 86),
		Item(divisors, 80),
		Item(divisors, 55),
		Item(divisors, 81)
		});
	items.push_back({
		Item(divisors, 73),
		Item(divisors, 68),
		Item(divisors, 73),
		Item(divisors, 90)
		});
	items.push_back({
		Item(divisors, 72),
		Item(divisors, 89),
		Item(divisors, 55),
		Item(divisors, 51),
		Item(divisors, 59)
		});
	items.push_back({
		Item(divisors, 76),
		Item(divisors, 76),
		Item(divisors, 91)
		});
	items.push_back({
		Item(divisors, 88)
		});
	items.push_back({
		Item(divisors, 64),
		Item(divisors, 63),
		Item(divisors, 56),
		Item(divisors, 50),
		Item(divisors, 77),
		Item(divisors, 55),
		Item(divisors, 55),
		Item(divisors, 86)
		});
	items.push_back({
		Item(divisors, 79),
		Item(divisors, 58)
		});

	return items;
}

void ProcessMonkey(std::vector<std::vector<Item>>& items, int monkeyNo, 
	std::function<void(Item&)> operation,
	int divisor, int trueDestination, int falseDestination)
{
	for (Item& item : items[monkeyNo])
	{
		operation(item);
		bool test = item.IsDivisible(monkeyNo);
		if (test)
			items[trueDestination].push_back(item);
		else
			items[falseDestination].push_back(item);
	}
	items[monkeyNo].clear();
}

void ProcessMonkeys(std::vector<std::vector<Item>>& items, std::vector<int>& numInspects)
{
	numInspects[0] += items[0].size();
	ProcessMonkey(items, 0, [](Item& item){ item.Multiply(17); }, 11, 2, 3);
	numInspects[1] += items[1].size();
	ProcessMonkey(items, 1, [](Item& item){ item.Add(7); }, 3, 6, 5 );
	numInspects[2] += items[2].size();
	ProcessMonkey(items, 2, [](Item& item){ item.Square(); }, 5, 1, 7 );
	numInspects[3] += items[3].size();
	ProcessMonkey(items, 3, [](Item& item){ item.Add(1); }, 7, 2, 7 );
	numInspects[4] += items[4].size();
	ProcessMonkey(items, 4, [](Item& item){ item.Multiply(3); }, 19, 0, 3 );
	numInspects[5] += items[5].size();
	ProcessMonkey(items, 5, [](Item& item){ item.Add(4); }, 2, 6, 4 );
	numInspects[6] += items[6].size();
	ProcessMonkey(items, 6, [](Item& item){ item.Add(8); }, 13, 4, 0 );
	numInspects[7] += items[7].size();
	ProcessMonkey(items, 7, [](Item& item){ item.Add(6); }, 17, 1, 5 );
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto items = InitializeItems();
	
	std::vector<int> numInspects;
	for (int i = 0; i < items.size(); i++)
		numInspects.push_back(0);

	for (int round = 0; round < NUM_ROUNDS; round++)
		ProcessMonkeys(items, numInspects);

	int max1 = 0;
	int max2 = 0;
	for (int n : numInspects)
	{
		if (n > max2)
		{
			max1 = max2;
			max2 = n;
		}
		else if (n > max1)
		{
			max1 = n;
		}
	}
	
	long result = (long)max1 * max2;	
	std::cout << "Result: " << result << "\n";
}
