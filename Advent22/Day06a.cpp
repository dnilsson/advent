#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day06.txt";

bool IsSequenceUnique(const std::string& indata, int sequenceLength, int lastPos)
{
	for (int pos1 = lastPos - sequenceLength + 1; pos1 < lastPos; pos1++)
		for (int pos2 = pos1 + 1; pos2 <= lastPos; pos2++)
			if (indata[pos1] == indata[pos2])
				return false;

	return true;
}

int GetLastPosOfFirstUniqueSequence(const std::string& indata, int sequenceLength)
{
	for (int lastPos = sequenceLength - 1; lastPos < indata.size(); lastPos++)
	{
		if (IsSequenceUnique(indata, sequenceLength, lastPos))
			return lastPos;
	}
	
	return -1;
}

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME)[0];
	std::cout << "Result: " << (1 + GetLastPosOfFirstUniqueSequence(indata, 4)) << "\n";
	return 0;
}
