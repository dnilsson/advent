#include "AdventUtil.h"
#include <iostream>
#include <vector>
#include <algorithm>

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
	
	std::sort(r.begin(), r.end());

	std::cout << "Answer: " << (r[r.size()-1] + r[r.size()-2] + r[r.size()-3]) << "\n";
	return 0;
}
