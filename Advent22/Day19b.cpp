#include "AdventUtil.h"

#include <iostream>
#include <unordered_map>


///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day19.txt";

const int NUM_RECIPES = 3;
const int PROCESS_TIME = 32;
const int MEMOIZE_LIMIT = 7;

std::unordered_map<uint64_t, int> stateToCreatedGeodes;


class State
{
private:
	uint64_t _state;
	
public:
	State(uint8_t numOreBots, uint8_t numClayBots, uint8_t numObsidianBots, uint8_t numGeodeBots,
		uint8_t amtOre, uint8_t amtClay, uint8_t amtObsidian, uint8_t remainingTime)
	{
		_state = (static_cast<uint64_t>(numOreBots) << 56)
			| (static_cast<uint64_t>(numClayBots) << 48)
			| (static_cast<uint64_t>(numObsidianBots) << 40)
			| (static_cast<uint64_t>(numGeodeBots) << 32)
			| (static_cast<uint64_t>(amtOre) << 24)
			| (static_cast<uint64_t>(amtClay) << 16)
			| (static_cast<uint64_t>(amtObsidian) << 8)
			| (static_cast<uint64_t>(remainingTime) << 0);
	}

	uint8_t GetNumOreBots() const { return static_cast<uint8_t>((_state & 0xff00000000000000) >> 56); }
	uint8_t GetNumClayBots() const { return static_cast<uint8_t>((_state & 0x00ff000000000000) >> 48); }
	uint8_t GetNumObsidianBots() const { return static_cast<uint8_t>((_state & 0x0000ff0000000000) >> 40); }
	uint8_t GetNumGeodeBots() const { return static_cast<uint8_t>((_state & 0x000000ff00000000) >> 32); }
	uint8_t GetAmtOre() const { return static_cast<uint8_t>((_state & 0x00000000ff000000) >> 24); }
	uint8_t GetAmtClay() const { return static_cast<uint8_t>((_state & 0x0000000000ff0000) >> 16); }
	uint8_t GetAmtObsidian() const { return static_cast<uint8_t>((_state & 0x000000000000ff00) >> 8); }
	uint8_t GetRemainingTime() const { return static_cast<uint8_t>((_state & 0x00000000000000ff) >> 0); }
	
	uint64_t GetState() const { return _state; }
	
	State CollectAndStepTime() const
	{
		return State( GetNumOreBots(), GetNumClayBots(), GetNumObsidianBots(), GetNumGeodeBots(),
			GetAmtOre() + GetNumOreBots(), GetAmtClay() + GetNumClayBots(), GetAmtObsidian() + GetNumObsidianBots(), GetRemainingTime() - 1);
	}
};


class Recipe
{
private:
	uint8_t oreBotCostInOre;
	uint8_t clayBotCostInOre;
	uint8_t obsidianBotCostInOre;
	uint8_t obsidianBotCostInClay;
	uint8_t geodeBotCostInOre;
	uint8_t geodeBotCostInObsidian;
	
public:
	Recipe(const std::string& s)
	{
		auto toks = AdventUtil::SplitString(s, ' ');
		oreBotCostInOre = std::atol(toks[6].c_str());
		clayBotCostInOre = std::atol(toks[12].c_str());
		obsidianBotCostInOre = std::atol(toks[18].c_str());
		obsidianBotCostInClay = std::atol(toks[21].c_str());
		geodeBotCostInOre = std::atol(toks[27].c_str());
		geodeBotCostInObsidian = std::atol(toks[30].c_str());
	}
	
	bool CanCreateOreBot(const State& state) const { return state.GetAmtOre() >= oreBotCostInOre; }
	bool CanCreateClayBot(const State& state) const { return state.GetAmtOre() >= clayBotCostInOre; }
	bool CanCreateObsidianBot(const State& state) const { return state.GetAmtOre() >= obsidianBotCostInOre && state.GetAmtClay() >= obsidianBotCostInClay; }
	bool CanCreateGeodeBot(const State& state) const { return state.GetAmtOre() >= geodeBotCostInOre && state.GetAmtObsidian() >= geodeBotCostInObsidian; }

	State CreateOreBot(const State& s) const
	{
		return State( s.GetNumOreBots() + 1, s.GetNumClayBots(), s.GetNumObsidianBots(), s.GetNumGeodeBots(),
			s.GetAmtOre() - oreBotCostInOre, s.GetAmtClay(), s.GetAmtObsidian(), s.GetRemainingTime() );
	}

	State CreateClayBot(const State& s) const
	{
		return State( s.GetNumOreBots(), s.GetNumClayBots() + 1, s.GetNumObsidianBots(), s.GetNumGeodeBots(),
			s.GetAmtOre() - clayBotCostInOre, s.GetAmtClay(), s.GetAmtObsidian(), s.GetRemainingTime() );
	}

	State CreateObsidianBot(const State& s) const
	{
		return State( s.GetNumOreBots(), s.GetNumClayBots(), s.GetNumObsidianBots() + 1, s.GetNumGeodeBots(),
			s.GetAmtOre() - obsidianBotCostInOre, s.GetAmtClay() - obsidianBotCostInClay, s.GetAmtObsidian(), s.GetRemainingTime() );
	}

	State CreateGeodeBot(const State& s) const
	{
		return State( s.GetNumOreBots(), s.GetNumClayBots(), s.GetNumObsidianBots(), s.GetNumGeodeBots() + 1,
			s.GetAmtOre() - geodeBotCostInOre, s.GetAmtClay(), s.GetAmtObsidian() - geodeBotCostInObsidian, s.GetRemainingTime() );
	}
};



///////////////////////////////////////////////////////////////////////////////
// Helper functions

int CalculateMaxGeodesCreated(const Recipe& recipe, const State& state)
{
	if (state.GetRemainingTime() == 0)
		return 0;
	else if (state.GetRemainingTime() == 1)
		return state.GetNumGeodeBots();
	else if (stateToCreatedGeodes.find(state.GetState()) != stateToCreatedGeodes.end())
		return stateToCreatedGeodes[state.GetState()];

	bool canCreateOreBot = recipe.CanCreateOreBot(state);
	bool canCreateClayBot = recipe.CanCreateClayBot(state);
	bool canCreateObsidianBot = recipe.CanCreateObsidianBot(state);
	bool canCreateGeodeBot = recipe.CanCreateGeodeBot(state);
	
	State nextState = state.CollectAndStepTime();
	int maxCreatedGeodes = 0;

	if (canCreateOreBot)
		maxCreatedGeodes = std::max(maxCreatedGeodes, CalculateMaxGeodesCreated(recipe, recipe.CreateOreBot(nextState)));

	if (canCreateClayBot)
		maxCreatedGeodes = std::max(maxCreatedGeodes, CalculateMaxGeodesCreated(recipe, recipe.CreateClayBot(nextState)));

	if (canCreateObsidianBot)
		maxCreatedGeodes = std::max(maxCreatedGeodes, CalculateMaxGeodesCreated(recipe, recipe.CreateObsidianBot(nextState)));

	if (canCreateGeodeBot)
		maxCreatedGeodes = std::max(maxCreatedGeodes, CalculateMaxGeodesCreated(recipe, recipe.CreateGeodeBot(nextState)));
	 
	maxCreatedGeodes = std::max(maxCreatedGeodes, CalculateMaxGeodesCreated(recipe, nextState));

	int result = maxCreatedGeodes + state.GetNumGeodeBots();
	if (state.GetRemainingTime() >= MEMOIZE_LIMIT)
		stateToCreatedGeodes.insert({ state.GetState(), result });
	return result;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	
	int product = 1;
	for (int i = 0; i < NUM_RECIPES; i++)
	{
		stateToCreatedGeodes.clear();
		Recipe recipe(lines[i]);
		State state(1, 0, 0, 0, 0, 0, 0, PROCESS_TIME);
		int n = CalculateMaxGeodesCreated(recipe, state);
		product *= n;
	}

	std::cout << "Result: " << product << "\n";
	return 0;
}
