#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day20.txt";

const long VALUE_MULTIPLIER = 811589153;
const int NUM_ROUNDS = 10;

struct Number
{
	long value;
	long initialIndex;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<Number> ParseNumbers(const std::vector<std::string>& lines)
{
	std::vector<Number> result;
	long idx = 0;
	for (const auto& line : lines)
		result.push_back({ std::atol(line.c_str()) * VALUE_MULTIPLIER, idx++ });
	return result;
}

long Modulo(long a, long b) 
{
	return a >= 0 ? a % b : ( b - abs ( a%b ) ) % b;
}

void MixNumber(std::vector<Number>& numbers, long initialIndex)
{
	long size = numbers.size();
	long currentIndex = 0;
	while (numbers[currentIndex].initialIndex != initialIndex)
		currentIndex++;
		
	long numSteps = numbers[currentIndex].value;
	numSteps = Modulo(numSteps, size * (size - 1));
	Number temp = numbers[currentIndex];
	
	if (numSteps > 0)
	{
		for (long i = 0; i < numSteps; i++)
			numbers[Modulo(currentIndex + i, size)] = numbers[Modulo(currentIndex + i + 1, size)];
	}
	else
	{
		for (long i = 0; i < -numSteps; i++)
			numbers[Modulo(currentIndex - i, size)] = numbers[Modulo(currentIndex - i - 1, size)];
	}

	numbers[Modulo(currentIndex + numSteps, size)] = temp;
}



///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto numbers = ParseNumbers(lines);

	for (int round = 0; round < NUM_ROUNDS; round++)
	{
		std::cout << "Mixing round " << round << "...\n";
		for (long i = 0; i < numbers.size(); i++)
			MixNumber(numbers, i);
	}
	
	long index = 0;
	while (numbers[index].value != 0)
		index++;
	
	index = Modulo(index + 1000, numbers.size());
	long a = numbers[index].value;

	index = Modulo(index + 1000, numbers.size());
	long b = numbers[index].value;
	
	index = Modulo(index + 1000, numbers.size());
	long c = numbers[index].value;

	std::cout << "Result: " << (a + b + c) << "\n";
	return 0;
}
