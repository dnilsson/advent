#include "AdventUtil.h"
#include <iostream>
#include <unordered_set>

///////////////////////////////////////////////////////////////////////////////
// Constants and types

const std::string FILENAME = "Day09.txt";
const int ROPE_LENGTH = 10;

enum Direction { Up, Down, Left, Right };

struct Instruction
{
      Direction direction;
      int numMoves;
};

struct Position
{
      int x;
      int y;
};


///////////////////////////////////////////////////////////////////////////////
// Parsing and preparation

std::vector<Instruction> ParseInstructions(const std::vector<std::string>& inputLines)
{
      std::vector<Instruction> result;
      
      for (const auto& line : inputLines)
      {
            auto parts = AdventUtil::SplitString(line);
            char d = parts[0][0];
            Direction direction = (d == 'U') ? Up
                  : (d == 'D') ? Down
                  : (d == 'L') ? Left
                  : Right;
            int numMoves = std::atol(parts[1].c_str());
            result.push_back({ direction, numMoves });
      }
      
      return result;
}

std::vector<Direction> UnpackInstructions(const std::vector<Instruction>& instructions)
{
      std::vector<Direction> result;
      
      for (const auto& instruction : instructions)
      {
            for (int i = 0; i < instruction.numMoves; i++)
                  result.push_back(instruction.direction);
      }
      
      return result;
}


///////////////////////////////////////////////////////////////////////////////
// Solution logic

void MoveTail(const Position& headPosition, Position& tailPosition)
{
      int distanceX = std::abs(tailPosition.x - headPosition.x);
      int distanceY = std::abs(tailPosition.y - headPosition.y);

      if (distanceX < 2 && distanceY < 2)
            return;

      int moveX = tailPosition.x > headPosition.x ? -1
            : tailPosition.x < headPosition.x ? 1
            : 0;

      int moveY = tailPosition.y > headPosition.y ? -1
            : tailPosition.y < headPosition.y ? 1
            : 0;

      tailPosition.x += moveX;
      tailPosition.y += moveY;
}

int CountVisitedTailPositions(const std::vector<Direction> headMoves)
{
      std::unordered_set<long> visitedTailPositions;

      Position positions[ROPE_LENGTH];
      for (int i = 0; i < ROPE_LENGTH; i++)
            positions[i] = { 0, 0 };

      for (auto headMove : headMoves)
      {
            switch (headMove)
            {
            case Up:
                  positions[0].y -= 1;
                  break;
            case Down:
                  positions[0].y += 1;
                  break;
            case Left:
                  positions[0].x -= 1;
                  break;
            case Right:
                  positions[0].x += 1;
                  break;
            }

			for (int i = 1; i < ROPE_LENGTH; i++)            
                  MoveTail(positions[i - 1], positions[i]);

			long positionId = positions[ROPE_LENGTH - 1].x + (static_cast<long>(1) << 32) * positions[ROPE_LENGTH - 1].y;            
            if (visitedTailPositions.find(positionId) == visitedTailPositions.end())
                  visitedTailPositions.insert(positionId);
      }

      return visitedTailPositions.size();
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
      auto lines = AdventUtil::ReadFile(FILENAME);
      std::cout << "Lines read: " << lines.size() << "\n";
      auto instructions = ParseInstructions(lines);
      std::cout << "Instruction extracted: " << instructions.size() << "\n";
      auto headMoves = UnpackInstructions(instructions);
      std::cout << "Moves unpacked: " << headMoves.size() << "\n";
      int result = CountVisitedTailPositions(headMoves);
      std::cout << "Result: " << result << "\n";
}
