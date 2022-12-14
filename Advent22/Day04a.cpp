#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day04.txt";

bool Envelops(const std::string& str)
{
	auto intervals = AdventUtil::SplitString(str, ',');
	auto interval1 = AdventUtil::SplitString(intervals[0], '-');
	auto interval2 = AdventUtil::SplitString(intervals[1], '-');
	
	int v1 = std::atol(interval1[0].c_str());
	int v2 = std::atol(interval1[1].c_str());
	int v3 = std::atol(interval2[0].c_str());
	int v4 = std::atol(interval2[1].c_str());
		
	return (v1 <= v3 && v2 >= v4) || (v3 <= v1 && v4 >= v2);
}

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME);
	std::cout << "Result: " << std::count_if(indata.cbegin(), indata.cend(), Envelops) << "\n";
	return 0;
}
