#include <iostream>
#include <vector>

const int NUM_ROUNDS = 20;

std::vector<std::vector<int>> InitializeItems()
{
	std::vector<std::vector<int>> items;
	
	// Real data
	items.push_back({ 56, 52, 58, 96, 70, 75, 72 });
	items.push_back({ 75, 58, 86, 80, 55, 81 });
	items.push_back({ 73, 68, 73, 90 });
	items.push_back({ 72, 89, 55, 51, 59 });
	items.push_back({ 76, 76, 91 });
	items.push_back({ 88 });
	items.push_back({ 64, 63, 56, 50, 77, 55, 55, 86 });
	items.push_back({ 79, 58 });

	// Test data	
	/*
	items.push_back({ 79, 98 });
	items.push_back({ 54, 65, 75, 74 });
	items.push_back({ 79, 60, 97 });
	items.push_back({ 74 });
	*/

	return items;
}

void DebugItems(const std::vector<std::vector<int>>& items)
{
	for (int monkeyNo = 0; monkeyNo < items.size(); monkeyNo++)
	{
		std::cout << "Monkey " << monkeyNo << ": ";
		const auto& monkeyItems = items[monkeyNo];
		bool first = true;
		for (const auto& item : monkeyItems)
		{
			if (!first)
				std::cout << ", ";
			std::cout << item;
			first = false;
		}
		std::cout << "\n";
	}
}

void DebugNumInspects(const std::vector<int>& numInspects)
{
	for (int monkeyNo = 0; monkeyNo < numInspects.size(); monkeyNo++)
	{
		std::cout << "Monkey " << monkeyNo << " inspected items " << numInspects[monkeyNo] << " times.\n";
	}
}

void ProcessMonkey(std::vector<std::vector<int>>& items, int monkeyNo, 
	std::function<void(int&)> operation, const std::string operationText,
	int divisor, int trueDestination, int falseDestination)
{
	std::cout << "Monkey " << monkeyNo << ":\n";
	for (int& item : items[monkeyNo])
	{
		std::cout << "  Monkey inspects an item with a worry level of " << item << ".\n";
		operation(item);
		std::cout << "    Worry level is " << operationText << " to " << item << ".\n";
		item /= 3;
		std::cout << "    Monkey gets bored with item. Worry level is divided by 3 to " << item << ".\n";
		bool test = (item % divisor == 0);
		if (test)
		{
			std::cout << "    Current worry level is divisible by " << divisor << ".\n";
			std::cout << "    Item with worry level " << item << " is thrown to monkey " << trueDestination << ".\n";
			items[trueDestination].push_back(item);
		}
		else
		{
			std::cout << "    Current worry level is not divisible by " << divisor << ".\n";
			std::cout << "    Item with worry level " << item << " is thrown to monkey " << falseDestination << ".\n";
			items[falseDestination].push_back(item);
		}
	}
	items[monkeyNo].clear();
}

void ProcessMonkeys(std::vector<std::vector<int>>& items, std::vector<int>& numInspects)
{
	// Real code
	numInspects[0] += items[0].size();
	ProcessMonkey(items, 0, [](int& item){ item *= 17; }, "multiplied by 17", 11, 2, 3);
	numInspects[1] += items[1].size();
	ProcessMonkey(items, 1, [](int& item){ item += 7; }, "increased by 7", 3, 6, 5 );
	numInspects[2] += items[2].size();
	ProcessMonkey(items, 2, [](int& item){ item = item * item; }, "multiplied by itself", 5, 1, 7 );
	numInspects[3] += items[3].size();
	ProcessMonkey(items, 3, [](int& item){ item += 1; }, "increased by 1", 7, 2, 7 );
	numInspects[4] += items[4].size();
	ProcessMonkey(items, 4, [](int& item){ item *= 3; }, "multiplied by 3", 19, 0, 3 );
	numInspects[5] += items[5].size();
	ProcessMonkey(items, 5, [](int& item){ item += 4; }, "increased by 4", 2, 6, 4 );
	numInspects[6] += items[6].size();
	ProcessMonkey(items, 6, [](int& item){ item += 8; }, "increased by 8", 13, 4, 0 );
	numInspects[7] += items[7].size();
	ProcessMonkey(items, 7, [](int& item){ item += 6; }, "increased by 6", 17, 1, 5 );
	
	// Test code
	/*
	numInspects[0] += items[0].size();
	ProcessMonkey(items, 0, [](int& item){ item *= 19; }, "multiplied by 19", 23, 2, 3);
	numInspects[1] += items[1].size();
	ProcessMonkey(items, 1, [](int& item){ item += 6; }, "increased by 6", 19, 2, 0 );
	numInspects[2] += items[2].size();
	ProcessMonkey(items, 2, [](int& item){ item = item * item; }, "multiplied by itself", 13, 1, 3 );
	numInspects[3] += items[3].size();
	ProcessMonkey(items, 3, [](int& item){ item += 3; }, "increased by 3", 17, 0, 1 );
	*/
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto items = InitializeItems();
	DebugItems(items);
	
	std::vector<int> numInspects;
	for (int i = 0; i < items.size(); i++)
		numInspects.push_back(0);

	for (int round = 0; round < NUM_ROUNDS; round++)
	{
		std::cout << "******* ROUND " << (round + 1) << " ********************\n\n";

		//for (int monkeyNo = 0; monkeyNo < items.size(); monkeyNo++)
		//	numInspects[monkeyNo] += items[monkeyNo].size();		// BUG: Must be done AFTER previous ProcessMonkey calls

		ProcessMonkeys(items, numInspects);

		std::cout << "\nAfter round " << (round + 1) << ", the monkeys are holding items with these worry levels:\n";
		DebugItems(items);
		std::cout << "\n";
	}

	DebugNumInspects(numInspects);

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
	
	std::cout << "Level of monkey business: " << result << "\n";
}
