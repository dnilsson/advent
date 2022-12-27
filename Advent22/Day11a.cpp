#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Constants

const int NUM_ROUNDS = 20;


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<std::vector<int>> InitializeItems()
{
	std::vector<std::vector<int>> items;
	
	items.push_back({ 56, 52, 58, 96, 70, 75, 72 });
	items.push_back({ 75, 58, 86, 80, 55, 81 });
	items.push_back({ 73, 68, 73, 90 });
	items.push_back({ 72, 89, 55, 51, 59 });
	items.push_back({ 76, 76, 91 });
	items.push_back({ 88 });
	items.push_back({ 64, 63, 56, 50, 77, 55, 55, 86 });
	items.push_back({ 79, 58 });

	return items;
}

void ProcessMonkey(std::vector<std::vector<int>>& items, int monkeyNo, 
	std::function<void(int&)> operation,
	int divisor, int trueDestination, int falseDestination)
{
	for (int& item : items[monkeyNo])
	{
		operation(item);
		item /= 3;
		bool test = (item % divisor == 0);
		if (test)
			items[trueDestination].push_back(item);
		else
			items[falseDestination].push_back(item);
	}
	
	items[monkeyNo].clear();
}

void ProcessMonkeys(std::vector<std::vector<int>>& items, std::vector<int>& numInspects)
{
	numInspects[0] += items[0].size();
	ProcessMonkey(items, 0, [](int& item){ item *= 17; }, 11, 2, 3);
	numInspects[1] += items[1].size();
	ProcessMonkey(items, 1, [](int& item){ item += 7; }, 3, 6, 5 );
	numInspects[2] += items[2].size();
	ProcessMonkey(items, 2, [](int& item){ item = item * item; }, 5, 1, 7 );
	numInspects[3] += items[3].size();
	ProcessMonkey(items, 3, [](int& item){ item += 1; }, 7, 2, 7 );
	numInspects[4] += items[4].size();
	ProcessMonkey(items, 4, [](int& item){ item *= 3; }, 19, 0, 3 );
	numInspects[5] += items[5].size();
	ProcessMonkey(items, 5, [](int& item){ item += 4; }, 2, 6, 4 );
	numInspects[6] += items[6].size();
	ProcessMonkey(items, 6, [](int& item){ item += 8; }, 13, 4, 0 );
	numInspects[7] += items[7].size();
	ProcessMonkey(items, 7, [](int& item){ item += 6; }, 17, 1, 5 );
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
	int result = max1 * max2;
	
	std::cout << "Result: " << result << "\n";
}
