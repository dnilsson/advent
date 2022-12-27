#include "AdventUtil.h"

#include <iostream>
#include <unordered_map>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day21.txt";

enum Operator { Add, Subtract, Multiply, Divide, Leaf };

struct Definition
{
	Operator op;
	std::string operand1;
	std::string operand2;	// Not for Leaf
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::unordered_map<std::string, Definition> ParseDefinitions(const std::vector<std::string>& lines)
{
	std::unordered_map<std::string, Definition> result;
	
	for (const auto& line : lines)
	{
		auto toks = AdventUtil::SplitString(line);
		auto symbol = toks[0].substr(0, toks[0].size() - 1);
		if (toks.size() == 2)
		{
			result.insert({ symbol, { Leaf, toks[1] } });
		}
		else
		{
			Operator op = toks[2] == "+" ? Add
				: toks[2] == "-" ? Subtract
				: toks[2] == "*" ? Multiply
				: Divide;
			result.insert({ symbol, { op, toks[1], toks[3] } });
		}
	}
	
	return result;
}

bool CanResolve(std::unordered_map<std::string, Definition>& definitions, const std::string& symbol)
{
	auto& definition = definitions[symbol];
	if (definition.op == Leaf)
		return definition.operand1 != "";

	return CanResolve(definitions, definition.operand1) && CanResolve(definitions, definition.operand2);		
}

long Resolve(std::unordered_map<std::string, Definition>& definitions, const std::string& symbol)
{
	auto& definition = definitions[symbol];
	
	switch (definition.op)
	{
	case Add:
		return Resolve(definitions, definition.operand1) + Resolve(definitions, definition.operand2);
	case Subtract:
		return Resolve(definitions, definition.operand1) - Resolve(definitions, definition.operand2);
	case Multiply:
		return Resolve(definitions, definition.operand1) * Resolve(definitions, definition.operand2);
	case Divide:
		return Resolve(definitions, definition.operand1) / Resolve(definitions, definition.operand2);
	default:
		return std::atol(definition.operand1.c_str());
	}
}

void FillBlank(std::unordered_map<std::string, Definition>& definitions, const std::string& symbol, long targetValue)
{
	auto& symbolDefinition = definitions[symbol];
	if (symbolDefinition.op == Leaf && symbolDefinition.operand1.empty())
	{
		symbolDefinition.operand1 = std::to_string(targetValue);
		return;
	}
	
	if (symbolDefinition.op == Add)
	{
		if (CanResolve(definitions, symbolDefinition.operand1))
		{
			auto op1Value = Resolve(definitions, symbolDefinition.operand1);
			FillBlank(definitions, symbolDefinition.operand2, targetValue - op1Value);
		}
		else
		{
			auto op2Value = Resolve(definitions, symbolDefinition.operand2);
			FillBlank(definitions, symbolDefinition.operand1, targetValue - op2Value);
		}
	}
	else if (symbolDefinition.op == Subtract)
	{
		if (CanResolve(definitions, symbolDefinition.operand1))
		{
			auto op1Value = Resolve(definitions, symbolDefinition.operand1);
			FillBlank(definitions, symbolDefinition.operand2, op1Value - targetValue);
		}
		else
		{
			auto op2Value = Resolve(definitions, symbolDefinition.operand2);
			FillBlank(definitions, symbolDefinition.operand1, targetValue + op2Value);
		}
	}
	else if (symbolDefinition.op == Multiply)
	{
		if (CanResolve(definitions, symbolDefinition.operand1))
		{
			auto op1Value = Resolve(definitions, symbolDefinition.operand1);
			FillBlank(definitions, symbolDefinition.operand2, targetValue / op1Value);
		}
		else
		{
			auto op2Value = Resolve(definitions, symbolDefinition.operand2);
			FillBlank(definitions, symbolDefinition.operand1, targetValue / op2Value);
		}
	}
	else if (symbolDefinition.op == Divide)
	{
		if (CanResolve(definitions, symbolDefinition.operand1))
		{
			auto op1Value = Resolve(definitions, symbolDefinition.operand1);
			FillBlank(definitions, symbolDefinition.operand2, op1Value / targetValue);
		}
		else
		{
			auto op2Value = Resolve(definitions, symbolDefinition.operand2);
			FillBlank(definitions, symbolDefinition.operand1, op2Value * targetValue);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto definitions = ParseDefinitions(lines);

	auto& humnDefinition = definitions["humn"];
	humnDefinition.operand1.clear();

	auto& rootDefinition = definitions["root"];
	if (CanResolve(definitions, rootDefinition.operand1))
	{
		auto targetValue = Resolve(definitions, rootDefinition.operand1);
		FillBlank(definitions, rootDefinition.operand2, targetValue);
	}
	else
	{
		auto targetValue = Resolve(definitions, rootDefinition.operand2);
		FillBlank(definitions, rootDefinition.operand1, targetValue);
	}

	long result = std::atol(humnDefinition.operand1.c_str());
	std::cout << "Result: " << result << "\n";
	return 0;
}
