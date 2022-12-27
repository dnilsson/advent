#include "AdventUtil.h"

#include <iostream>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day13.txt";

enum TokenType { StartToken, EndToken, NumberToken, CommaToken };

struct Token
{
	TokenType type;
	int value;			// Only for NumberToken
};

enum NodeType { NumberNode, SubListNode };

struct Node
{
	NodeType type;
	int value;						// Only for NumberNode
	std::vector<Node> subNodes;		// Only for SubListNode
};


///////////////////////////////////////////////////////////////////////////////
// Helper functions

std::vector<Token> Tokenize(const std::string& line)
{
	std::vector<Token> result;
	
	for (int pos = 0; pos < line.size(); pos++)
	{
		if (line[pos] == '[')
			result.push_back({ StartToken, 0 });
		else if (line[pos] == ']')
			result.push_back({ EndToken, 0 });
		else if (line[pos] == ',')
			result.push_back({ CommaToken, 0 });
		else
		{
			int v = 0;
			while (pos < line.size() && line[pos] >= '0' && line[pos] <= '9')
			{
				v = 10 * v + (int)(line[pos] - '0');
				pos++;
			}
			result.push_back({ NumberToken, v });
			pos--;
		}
	}
	
	return result;
}

Node Parse(const std::vector<Token>& tokens, int& position)
{
	Node result{ SubListNode, 0 };
	position++;
	
	while (tokens[position].type != EndToken)
	{
		if (tokens[position].type == NumberToken)
		{
			result.subNodes.push_back(Node({ NumberNode, tokens[position].value }));
			position++;
		}
		else if (tokens[position].type == StartToken)
		{
			result.subNodes.push_back(Parse(tokens, position));
		}
		else
		{
			position++;
		}
	}

	position++;
	return result;
}

// Returns 1 if the left list is the greater, -1 if the right list is the greater, and 0 if they are equal
int CompareSubLists(const Node& leftNode, const Node& rightNode)
{
	auto size = std::max(leftNode.subNodes.size(), rightNode.subNodes.size());
	for (int i = 0; i < size; i++)
	{
		if (i >= leftNode.subNodes.size())
		{
			return -1;	// More nodes in the right list
		}
		else if (i >= rightNode.subNodes.size())
		{
			return 1;	// More nodes in the left list
		}
		else if (leftNode.subNodes[i].type == SubListNode && rightNode.subNodes[i].type == SubListNode)
		{
			int r = CompareSubLists(leftNode.subNodes[i], rightNode.subNodes[i]);
			if (r != 0)
				return r;
		}
		else if (leftNode.subNodes[i].type == NumberNode && rightNode.subNodes[i].type == NumberNode)
		{
			if (leftNode.subNodes[i].value > rightNode.subNodes[i].value)
				return 1;
			else if (rightNode.subNodes[i].value > leftNode.subNodes[i].value)
				return -1;
		}
		else if (leftNode.subNodes[i].type == SubListNode && rightNode.subNodes[i].type == NumberNode)
		{
			Node tempList{ SubListNode, 0 };
			tempList.subNodes.push_back(Node{ NumberNode, rightNode.subNodes[i].value });
			int r = CompareSubLists(leftNode.subNodes[i], tempList);
			if (r != 0)
				return r;
		}
		else if (leftNode.subNodes[i].type == NumberNode && rightNode.subNodes[i].type == SubListNode)
		{
			Node tempList{ SubListNode, 0 };
			tempList.subNodes.push_back(Node{ NumberNode, leftNode.subNodes[i].value });
			int r = CompareSubLists(tempList, rightNode.subNodes[i]);
			if (r != 0)
				return r;
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);
	int numListPairs = (lines.size() + 1) / 3;
	std::vector<Node> leftNodes;
	std::vector<Node> rightNodes;
	
	for (int i = 0; i < numListPairs; i++)
	{
		{
			auto line = lines[i * 3];
			auto tokens = Tokenize(line);		
			int pos = 0;
			auto node = Parse(tokens, pos);
			leftNodes.push_back(node);
		}		
		{
			auto line = lines[i * 3 + 1];
			auto tokens = Tokenize(line);
			int pos = 0;
			auto node = Parse(tokens, pos);
			rightNodes.push_back(node);
		}
	}

	int correctOrderIndiceSum = 0;
	
	for (int i = 0; i < numListPairs; i++)
	{
		if (CompareSubLists(leftNodes[i], rightNodes[i]) < 0)
			correctOrderIndiceSum += (i + 1);
	}
	
	std::cout << "Result: " << correctOrderIndiceSum << "\n";
}
