#include "AdventUtil.h"

#include <iostream>
#include <deque>

struct Instruction
{
	Instruction(int c, int f, int t) : count(c), fromPos(f), toPos(t) {}

	int count;
	int fromPos;
	int toPos;
};

void Initialize(const std::vector<std::string>& text, std::vector<std::deque<char>>& board, std::vector<Instruction>& instructions)
{
	for (int posNo = 0; posNo < 9; posNo++)
		board.push_back(std::deque<char>());

	for (int lineNo = 0; lineNo < 8; lineNo++)
	{
		auto s = text[lineNo];
		for (int posNo = 0; posNo < 9; posNo++)
		{
			auto c = s[posNo * 4 + 1];			
			if (c != ' ')
				board[posNo].push_back(c);
		}
	}

	for (int posNo = 0; posNo < 9; posNo++)
		std::reverse(board[posNo].begin(), board[posNo].end());

	for (int lineNo = 10; lineNo < 511; lineNo++)
	{
		auto s = text[lineNo];
		auto ss = AdventUtil::SplitString(s);
		instructions.emplace_back(std::atol(ss[1].c_str()), std::atol(ss[3].c_str()) - 1, std::atol(ss[5].c_str()) - 1);
	}
}

void Execute(std::vector<std::deque<char>>& board, const std::vector<Instruction>& instructions)
{
	for (const auto& instruction : instructions)
	{
		for (int i = 0; i < instruction.count; i++)
		{
			auto c = board[instruction.fromPos].back();
			board[instruction.fromPos].pop_back();
			board[instruction.toPos].push_back(c);
		}
	}
}

int main()
{
	auto lines = AdventUtil::ReadFile("Day05.txt");
	
	std::vector<std::deque<char>> board;
	std::vector<Instruction> instructions;

	Initialize(lines, board, instructions);
	Execute(board, instructions);
	
	std::cout << "Result: ";
	for (const auto& pos : board)
		std::cout << pos.back();
	std::cout << "\n";

	return 0;
}
