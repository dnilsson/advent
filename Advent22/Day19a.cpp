#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day19.txt";

struct Recipe
{
	int oreBotCostInOre;
	int clayBotCostInOre;
	int obsidianBotCostInOre;
	int obsidianBotCostInClay;
	int geodeBotCostInOre;
	int geodeBotCostInObsidian;
};

struct State
{
	int numOreBots;
	int numClayBots;
	int numObsidianBots;
	int numGeodeBots;
	int amtOre;
	int amtClay;
	int amtObsidian;
	int amtGeode;
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool CanCreateOreBot(const Recipe& recipe, const State& state)
{
	return state.amtOre >= recipe.oreBotCostInOre;
}

bool CanCreateClayBot(const Recipe& recipe, const State& state)
{
	return state.amtOre >= recipe.clayBotCostInOre;
}

bool CanCreateObsidianBot(const Recipe& recipe, const State& state)
{
	return state.amtOre >= recipe.obsidianBotCostInOre && state.amtClay >= recipe.obsidianBotCostInClay;
}

bool CanCreateGeodeBot(const Recipe& recipe, const State& state)
{
	return state.amtOre >= recipe.geodeBotCostInOre && state.amtObsidian >= recipe.geodeBotCostInObsidian;
}

State CreateOreBot(const Recipe& r, const State& s)
{
	return State{ s.numOreBots + 1, s.numClayBots, s.numObsidianBots, s.numGeodeBots,
		s.amtOre - r.oreBotCostInOre, s.amtClay, s.amtObsidian, s.amtGeode };
}

State CreateClayBot(const Recipe& r, const State& s)
{
	return State{ s.numOreBots, s.numClayBots + 1, s.numObsidianBots, s.numGeodeBots,
		s.amtOre - r.clayBotCostInOre, s.amtClay, s.amtObsidian, s.amtGeode };
}

State CreateObsidianBot(const Recipe& r, const State& s)
{
	return State{ s.numOreBots, s.numClayBots, s.numObsidianBots + 1, s.numGeodeBots,
		s.amtOre - r.obsidianBotCostInOre, s.amtClay - r.obsidianBotCostInClay, s.amtObsidian, s.amtGeode };
}

State CreateGeodeBot(const Recipe& r, const State& s)
{
	return State{ s.numOreBots, s.numClayBots, s.numObsidianBots, s.numGeodeBots + 1,
		s.amtOre - r.geodeBotCostInOre, s.amtClay, s.amtObsidian - r.geodeBotCostInObsidian, s.amtGeode };
}

int GetMaxNumGeodes(const Recipe& recipe, State state, int remainingTime)
{
	if (remainingTime == 0)
		return state.amtGeode;

	State initialState = state;
	state.amtOre += state.numOreBots;
	state.amtClay += state.numClayBots;
	state.amtObsidian += state.numObsidianBots;
	state.amtGeode += state.numGeodeBots;
		
	int result = 0;
	
	if (CanCreateOreBot(recipe, initialState))
		result = std::max(result, GetMaxNumGeodes(recipe, CreateOreBot(recipe, state), remainingTime - 1));

	if (CanCreateClayBot(recipe, initialState))
		result = std::max(result, GetMaxNumGeodes(recipe, CreateClayBot(recipe, state), remainingTime - 1));
	
	if (CanCreateObsidianBot(recipe, initialState))
		result = std::max(result, GetMaxNumGeodes(recipe, CreateObsidianBot(recipe, state), remainingTime - 1));
	
	if (CanCreateGeodeBot(recipe, initialState))
		result = std::max(result, GetMaxNumGeodes(recipe, CreateGeodeBot(recipe, state), remainingTime - 1));

	result = std::max(result, GetMaxNumGeodes(recipe, state, remainingTime - 1));
	
	return result;
}

Recipe ParseRecipe(const std::string& s)
{
	Recipe r;
	auto toks = AdventUtil::SplitString(s, ' ');
	r.oreBotCostInOre = std::atol(toks[6].c_str());
	r.clayBotCostInOre = std::atol(toks[12].c_str());
	r.obsidianBotCostInOre = std::atol(toks[18].c_str());
	r.obsidianBotCostInClay = std::atol(toks[21].c_str());
	r.geodeBotCostInOre = std::atol(toks[27].c_str());
	r.geodeBotCostInObsidian = std::atol(toks[30].c_str());
	return r;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	int recipeId = 0;
	int sumQuality = 0;
	for (const auto& line : lines)
	{
		recipeId++;
		Recipe recipe = ParseRecipe(line);
		State state{ 1, 0, 0, 0, 0, 0, 0, 0 };
		int numGeodes = GetMaxNumGeodes(recipe, state, 24);
		int quality = recipeId * numGeodes;
		sumQuality += quality;
	}

	std::cout << "Result: " << sumQuality << "\n";
	return 0;
}
