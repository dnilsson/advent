#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Constants and types

const std::string FILENAME = "Day10.txt";

const int CRT_WIDTH = 40;

enum OpCode { Addx, Noop };

struct Instruction
{
	OpCode opCode;
	int operand;	// Only for Addx
};


///////////////////////////////////////////////////////////////////////////////
// Parsing

std::vector<Instruction> ParseInstructions(const std::vector<std::string>& lines)
{
	std::vector<Instruction> result;
	
	for (const std::string& line : lines)
	{
		auto parts = AdventUtil::SplitString(line);
		if (parts[0] == "addx")
		{
			int operand = std::atol(parts[1].c_str());
			result.push_back({ Addx, operand });
		}
		else if (parts[0] == "noop")
		{
			result.push_back({ Noop, 0 });
		}
	}
	
	return result;
}


///////////////////////////////////////////////////////////////////////////////
// "Computer"

class Computer
{
private:
	const std::vector<Instruction>& _instructions;
	std::vector<Instruction>::const_iterator _currentInstruction;
	int _remainingCyclesForCurrentInstruction = 0;
	int _currentValue = 1;

public:
	Computer(const std::vector<Instruction>& instructions)
		: _instructions(instructions)
		, _currentInstruction(instructions.cbegin())
	{ 
		_remainingCyclesForCurrentInstruction = _currentInstruction->opCode == Addx ? 2 : 1;
	}

	void Tick()
	{
		if (_currentInstruction == _instructions.cend())
			return;
	
		_remainingCyclesForCurrentInstruction--;
		if (_remainingCyclesForCurrentInstruction)
			return;
			
		if (_currentInstruction->opCode == Addx)
			_currentValue += _currentInstruction->operand;
			
		_currentInstruction++;
		if (_currentInstruction == _instructions.cend())
			return;
			
		_remainingCyclesForCurrentInstruction = _currentInstruction->opCode == Addx ? 2 : 1;
	}
	
	int GetCurrentValue()
	{
		return _currentValue;
	}
	
	bool IsFinished()
	{
		return _currentInstruction == _instructions.cend();
	}
};


///////////////////////////////////////////////////////////////////////////////
// Pixel check

bool IsPixelLit(int cycleNo, int computerOutput)
{
	int column = cycleNo % CRT_WIDTH;
	return (std::abs(column - computerOutput) < 2);
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto instructions = ParseInstructions(lines);
	
	Computer computer(instructions);
	int cycleNo = 0;
	
	while (!computer.IsFinished())
	{
		std::cout << (IsPixelLit(cycleNo, computer.GetCurrentValue()) ? "#" : ".");

		cycleNo++;
		computer.Tick();

		if (cycleNo % CRT_WIDTH == 0)
			std::cout << "\n";
	}
}
