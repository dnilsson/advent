#include "AdventUtil.h"

#include <iostream>
#include <queue>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day16.txt";
const std::string START_VALVE = "AA";

const int NUM_VALVES = 58;
const int TOTAL_TIME = 26;

struct Valve
{
	std::string name;
	int pressure;
	int remainingTimeAtOpen = -1;
	std::vector<int> adjacentValveIndexes;
	int distancesToValveIndexes[NUM_VALVES];
};


///////////////////////////////////////////////////////////////////////////////
// Parsing and preparing

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
		startValve.distancesToValveIndexes[i] = -1;

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
			if (startValve.distancesToValveIndexes[toValveIndex] != -1)
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

void ArriveAtValve(std::vector<Valve>& valves, int& maxReleasedPressure, int arrivalTime1, int valveIndex1, int arrivalTime2, int valveIndex2)
{
	bool walkedFurther = false;
	
	if (arrivalTime1 >= arrivalTime2 && arrivalTime1 != -1)
	{	
		Valve& valve = valves[valveIndex1];
		for (int i = 0; i < NUM_VALVES; i++)
		{
			if (valves[i].pressure == 0 || valves[i].remainingTimeAtOpen != -1)
				continue;
			
			int nextArrivalTime = arrivalTime1 - (valve.pressure != 0 ? 1 : 0) - valve.distancesToValveIndexes[i];
			if (nextArrivalTime <= 1)
				continue;
			
			valves[i].remainingTimeAtOpen = nextArrivalTime - 1;	// Set in advance, to indicate that the other "player" should ignore it
			ArriveAtValve(valves, maxReleasedPressure, nextArrivalTime, i, arrivalTime2, valveIndex2);
			valves[i].remainingTimeAtOpen = -1;
			walkedFurther = true;
		}
		if (!walkedFurther && arrivalTime2 != -1)
			ArriveAtValve(valves, maxReleasedPressure, -1, -1, arrivalTime2, valveIndex2);
	}
	else if (arrivalTime2 != -1)
	{
		Valve& valve = valves[valveIndex2];
		for (int i = 0; i < NUM_VALVES; i++)
		{
			if (valves[i].pressure == 0 || valves[i].remainingTimeAtOpen != -1)
				continue;
			
			int nextArrivalTime = arrivalTime2 - (valve.pressure != 0 ? 1 : 0) - valve.distancesToValveIndexes[i];
			if (nextArrivalTime <= 1)
				continue;
			
			valves[i].remainingTimeAtOpen = nextArrivalTime - 1;	// Set in advance, to indicate that the other "player" should ignore it
			ArriveAtValve(valves, maxReleasedPressure, arrivalTime1, valveIndex1, nextArrivalTime, i);
			valves[i].remainingTimeAtOpen = -1;
			walkedFurther = true;
		}
		if (!walkedFurther && arrivalTime1 != -1)
			ArriveAtValve(valves, maxReleasedPressure, arrivalTime1, valveIndex1, -1, -1);
	}

	if (!walkedFurther)
	{
		int totalPressure = 0;
		for (int i  = 0; i < NUM_VALVES; i++)
		{
			if (valves[i].pressure != 0 && valves[i].remainingTimeAtOpen != -1)
				totalPressure += valves[i].pressure * valves[i].remainingTimeAtOpen;
		}
		
		if (totalPressure > maxReleasedPressure)
			maxReleasedPressure = totalPressure;
	}
}

int GetMaxReleasedPressure(std::vector<Valve>& valves, int startValveIndex)
{
	int maxReleasedPressure = 0;
	ArriveAtValve(valves, maxReleasedPressure, TOTAL_TIME, startValveIndex, TOTAL_TIME, startValveIndex);
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
