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
	const std::string operand1;
	const std::string operand2;	// Not for Leaf
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


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	auto definitions = ParseDefinitions(lines);

	std::cout << "Result: " << Resolve(definitions, "root") << "\n";
	return 0;
}
