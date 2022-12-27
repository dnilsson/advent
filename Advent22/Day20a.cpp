#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day20.txt";

struct Number
{
	int value;
	int initialIndex;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<Number> ParseNumbers(const std::vector<std::string>& lines)
{
	std::vector<Number> result;
	int idx = 0;
	for (const auto& line : lines)
		result.push_back({ static_cast<int>(std::atol(line.c_str())), idx++ });
	return result;
}

void MixNumber(std::vector<Number>& numbers, int initialIndex)
{
	int size = numbers.size();
	int currentIndex = 0;
	while (numbers[currentIndex].initialIndex != initialIndex)
		currentIndex++;
		
	int numSteps = numbers[currentIndex].value;
	Number temp = numbers[currentIndex];
	
	if (numSteps > 0)
	{
		for (int i = 0; i < numSteps; i++)
		{
			int srcIndex = currentIndex + i + 1;
			while (srcIndex >= size)
				srcIndex -= size;
			int destIndex = currentIndex + i;
			while (destIndex >= size)
				destIndex -= size;
			numbers[destIndex] = numbers[srcIndex];
		}
		int newIndex = currentIndex + numSteps;
		while (newIndex >= size)
			newIndex -= size;
		numbers[newIndex] = temp;
	}
	else
	{
		for (int i = 0; i < -numSteps; i++)
		{
			int srcIndex = currentIndex - i - 1;
			while (srcIndex < 0)
				srcIndex += size;
			int destIndex = currentIndex - i;
			while (destIndex < 0)
				destIndex += size;
			numbers[destIndex] = numbers[srcIndex];
		}
		int newIndex = currentIndex + numSteps;
		while (newIndex < 0)
			newIndex += size;
		numbers[newIndex] = temp;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	
	auto numbers = ParseNumbers(lines);
	for (int i = 0; i < numbers.size(); i++)
		MixNumber(numbers, i);
	
	int index = 0;
	while (numbers[index].value != 0)
		index++;
	
	index += 1000;
	while (index >= numbers.size())
		index -= numbers.size();
	int a = numbers[index].value;

	index += 1000;
	while (index >= numbers.size())
		index -= numbers.size();
	int b = numbers[index].value;
	
	index += 1000;
	while (index >= numbers.size())
		index -= numbers.size();
	int c = numbers[index].value;

	std::cout << "Result: " << (a + b + c) << "\n";
	return 0;
}
