#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day03.txt";

int CalculatePrio(const std::string& contents)
{
	int size = contents.size() / 2;
	
	auto compartment1 = contents.substr(0, size);
	auto compartment2 = contents.substr(size, size);

	for (int i1 = 0; i1 < size; i1++)
	{
		for (int i2 = 0; i2 < size; i2++)
		{
			if (compartment1[i1] == compartment2[i2])
			{
				char item = compartment1[i1];
				int prio = (item >= 'a' && item <= 'z') ? (item - 'a' + 1)
					: (item >= 'A' && item <= 'Z') ? (item - 'A' + 27)
					: 0;

				return prio;
			}
		}
	}

	return 0;
}

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME);

	int totalPrio = 0;
	for (int i = 0; i < indata.size(); i++)
		totalPrio += CalculatePrio(indata[i]);
		
	std::cout << "Result: " << totalPrio << "\n";
	return 0;
}
