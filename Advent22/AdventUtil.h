#pragma once

#include <string>
#include <vector>
#include <fstream>

class AdventUtil
{
public:
	static std::vector<std::string> SplitString(const std::string& str, char delimiter = ' ')
	{
		std::vector<std::string> result;

		int startIdx = 0;
		int endIdx = 0;

		for (;;)
		{
			while (endIdx < str.size() && str[endIdx] != delimiter)
				endIdx++;

			result.push_back(str.substr(startIdx, endIdx - startIdx));

			if (endIdx == str.size())
				break;

			endIdx++;
			startIdx = endIdx;
		}

		return result;
	}

	static std::vector<std::string> ReadFile(const std::string& filename)
	{
		std::vector<std::string> result;
		std::ifstream infile(filename);
		std::string line;

		while (std::getline(infile, line))
			result.push_back(line);

		return result;
	}
};
