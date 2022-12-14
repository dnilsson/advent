#include "AdventUtil.h"
#include <iostream>

const std::string FILENAME = "Day02.txt";

int main()
{
	auto indata = AdventUtil::ReadFile(FILENAME);	

	enum Shape { Rock, Paper, Scissors };
	enum Outcome { Lose, Draw, Win };

	int totalScore = 0;
	
	for (int i = 0; i < 2500; i++)
	{
		const auto& entry = indata[i];
		
		char s = entry[0];
		Shape theirShape =
			s == 'A' ? Rock
			: s == 'B' ? Paper
			: Scissors;	// 'C'

		char o = entry[2];
		Outcome outcome =
			o == 'X' ? Lose
			: o == 'Y' ? Draw
			: Win;	// 'Z'

		Shape myShape =
			(theirShape == Rock && outcome == Lose) ? Scissors
			: (theirShape == Rock && outcome == Draw) ? Rock
			: (theirShape == Rock && outcome == Win) ? Paper
			: (theirShape == Paper && outcome == Lose) ? Rock
			: (theirShape == Paper && outcome == Draw) ? Paper
			: (theirShape == Paper && outcome == Win) ? Scissors
			: (theirShape == Scissors && outcome == Lose) ? Paper
			: (theirShape == Scissors && outcome == Draw) ? Scissors
			: (theirShape == Scissors && outcome == Win) ? Rock
			: Rock;
		
		int shapeScore =
			myShape == Rock ? 1
			: myShape == Paper ? 2
			: myShape == Scissors ? 3
			: 0;

		int outcomeScore =
			outcome == Lose ? 0
			: outcome == Draw ? 3
			: outcome == Win ? 6
			: 0;
	
		totalScore += shapeScore + outcomeScore;
	}

	std::cout << "Final score: " << totalScore << "\n";
	return 0;
}
