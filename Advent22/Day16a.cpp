#include "AdventUtil.h"

#include <iostream>
#include <queue>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day16.txt";
const std::string START_VALVE = "AA";

const int NUM_VALVES = 58;
const int TOTAL_TIME = 30;

struct Valve
{
	std::string name;
	int pressure;
	bool isOpen = false;
	std::vector<int> adjacentValveIndexes;
	int distancesToValveIndexes[NUM_VALVES];
};

std::stack<int> maxStack;
std::stack<int> maxTimeStack;


///////////////////////////////////////////////////////////////////////////////
// Helper functions

int GetValveIndex(std::vector<Valve>& valves, const std::string& name)
{
	for (int i = 0; i < NUM_VALVES; i++)
	{
		if (valves[i].name == name)
			return i;
	}

	return -1;
}

void BuildDistances(int startValveIndex, std::vector<Valve>& valves)
{
	auto& startValve = valves[startValveIndex];
	for (int i = 0; i < NUM_VALVES; i++)
		startValve.distancesToValveIndexes[i] = -100;

	startValve.distancesToValveIndexes[startValveIndex] = 0;
	std::queue<int> valveIndexQueue;
	valveIndexQueue.push(startValveIndex);
	
	while (!valveIndexQueue.empty())
	{
		auto fromValveIndex = valveIndexQueue.front();
		valveIndexQueue.pop();
		auto& fromValve = valves[fromValveIndex];
		
		for (auto toValveIndex : fromValve.adjacentValveIndexes)
		{
			if (startValve.distancesToValveIndexes[toValveIndex] != -100)
				continue;
			startValve.distancesToValveIndexes[toValveIndex] = startValve.distancesToValveIndexes[fromValveIndex] + 1;
			valveIndexQueue.push(toValveIndex);
		}
	}
}

std::vector<Valve> ParseValves(const std::vector<std::string>& lines)
{
	std::vector<Valve> valves;

	for (const auto& line : lines)
	{
		auto tokens = AdventUtil::SplitString(line);
		auto name = tokens[1];
		auto p = AdventUtil::SplitString(tokens[4], '=')[1];
		int pressure = std::atol(p.substr(0, p.size() - 1).c_str());
		valves.push_back({ name, pressure });
	}

	for (int i = 0; i < lines.size(); i++)
	{
		auto tokens = AdventUtil::SplitString(lines[i]);
		for (int j = 9; j < tokens.size(); j++)
		{
			auto destinationName = tokens[j];
			if (destinationName[destinationName.size() - 1] == ',')
				destinationName = destinationName.substr(0, destinationName.size() - 1);
			valves[i].adjacentValveIndexes.push_back(GetValveIndex(valves, destinationName));
		}
	}
			
	for (int i = 0; i < NUM_VALVES; i++)	
		BuildDistances(i, valves);	

	return valves;
}

void ArriveAtValve(std::vector<Valve>& valves, int& maxReleasedPressure, int releasedPressureSoFar,
	int remainingTime, int valveIndex, std::stack<int>& indexStack, std::stack<int>& timeStack)
{
	indexStack.push(valveIndex);
	timeStack.push(remainingTime);
	
	Valve& valve = valves[valveIndex];
	if (valve.pressure != 0)
	{
		remainingTime -= 1;
		releasedPressureSoFar += valve.pressure * remainingTime;
		valve.isOpen = true;
	}

	bool walkedFurther = false;
	for (int i = 0; i < NUM_VALVES; i++)
	{
		if (valves[i].pressure == 0 || valves[i].isOpen)
			continue;
			
		int remainingTimeAfterWalk = remainingTime - valve.distancesToValveIndexes[i];
		if (remainingTimeAfterWalk <= 1)
			continue;
			
		ArriveAtValve(valves, maxReleasedPressure, releasedPressureSoFar, remainingTimeAfterWalk, i, indexStack, timeStack);
		walkedFurther = true;
	}

	valve.isOpen = false;

	if (!walkedFurther)
	{
		if (releasedPressureSoFar > maxReleasedPressure)
		{
			maxReleasedPressure = releasedPressureSoFar;
			maxStack = indexStack;
			maxTimeStack = timeStack;
		}
	}
	
	timeStack.pop();
	indexStack.pop();
}

int GetMaxReleasedPressure(std::vector<Valve>& valves, int startValveIndex)
{
	std::stack<int> indexStack;
	std::stack<int> timeStack;
	int maxReleasedPressure = 0;
	ArriveAtValve(valves, maxReleasedPressure, 0, TOTAL_TIME, startValveIndex, indexStack, timeStack);
	return maxReleasedPressure;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{	
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto valves = ParseValves(lines);

	int startValveIndex = GetValveIndex(valves, START_VALVE);
	auto maxReleasedPressure = GetMaxReleasedPressure(valves, startValveIndex);

	std::cout << "Result: " << maxReleasedPressure << "\n";
	return 0;
}
