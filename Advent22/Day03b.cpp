#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day03.txt";

int GetPrio(char item)
{
	int prio = (item >= 'a' && item <= 'z') ? (item - 'a' + 1)
		: (item >= 'A' && item <= 'Z') ? (item - 'A' + 27)
		: 0;

	return prio;
}

int CalculateCommonItemPrio(const std::string& contents1, const std::string& contents2, const std::string& contents3)
{
	for (int i1 = 0; i1 < contents1.size(); i1++)
	{
		for (int i2 = 0; i2 < contents2.size(); i2++)
		{
			for (int i3 = 0; i3 < contents3.size(); i3++)
			{
				if (contents1[i1] == contents2[i2] && contents2[i2] == contents3[i3])
					return GetPrio(contents1[i1]);
			}
		}
	}

	return 0;
}

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME);

	int totalPrio = 0;
	for (int i = 0; i < indata.size(); i += 3)
		totalPrio += CalculateCommonItemPrio(indata[i], indata[i + 1], indata[i + 2]);
		
	std::cout << "Result: " << totalPrio << "\n";
	return 0;
}
