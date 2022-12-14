#include "AdventUtil.h"
#include <iostream>
#include <vector>

const std::string FILENAME = "Day01.txt";

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);

	std::vector<int> r;
	int acc = 0;

	for (const auto& line : lines)
	{
		if (line == "")
		{
			r.push_back(acc);
			acc = 0;
		}
		else
		{
			acc += std::atol(line.c_str());
		}
	}

	if (acc)
		r.push_back(acc);
	
	int max = 0;
	for (int i = 0; i < r.size(); i++)
	{
		if (r[i] > max)
			max = r[i];
	}
	
	std::cout << "Answer: " << max << "\n";
	return 0;
}
