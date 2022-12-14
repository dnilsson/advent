#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day02.txt";

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME);	
	int totalScore = 0;
	
	for (int i = 0; i < indata.size(); i++)
	{
		const auto& entry = indata[i];
		char theirShape = entry[0];
		char myShape = entry[2];
		
		int shapeScore =
			myShape == 'X' ? 1		// X = Rock
			: myShape == 'Y' ? 2	// Y = Paper
			: 3;					// Z = Scissors
		
		int winScore =
			(myShape == 'X' && theirShape == 'C') ? 6		// Rock beats scissors
			: (myShape == 'Y' && theirShape == 'A') ? 6		// Paper beats rock
			: (myShape == 'Z' && theirShape == 'B') ? 6		// Scissors beats paper
			: (myShape == 'X' && theirShape == 'B') ? 0		// Rock loses against paper
			: (myShape == 'Y' && theirShape == 'C') ? 0		// Paper loses against scissors
			: (myShape == 'Z' && theirShape == 'A') ? 0		// Scissors loses against rock
			: 3;											// Draw
	
		totalScore += shapeScore + winScore;
	}

	std::cout << "Final score: " << totalScore << "\n";
	return 0;
}
