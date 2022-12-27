#include "AdventUtil.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Types and constants

const std::string FILENAME = "Day25.txt";


///////////////////////////////////////////////////////////////////////////////
// Helpers

long SnafuToLong(const std::string& snafu)
{
	long result = 0;

	for (int i = 0; i < snafu.size(); i++)
	{
		result = 5 * result;
		char c = snafu[i];
		if (c == '=')
			result -= 2;
		else if (c == '-')
			result -= 1;
		else if (c == '1')
			result += 1;
		else if (c == '2')
			result += 2;
	}
	
	return result;
}

std::string LongToSnafu(long n)
{
	std::string s;

	while (n > 0)
	{
		long a = (n + 2) % 5;

		if (a == 0)
			s = "=" + s;
		else if (a == 1)
			s = "-" + s;
		else if (a == 2)
			s = "0" + s;
		else if (a == 3)
			s = "1" + s;
		else
			s = "2" + s;
			
		n = n - (a - 2);
		n = n / 5;
	}

	return s;
}


///////////////////////////////////////////////////////////////////////////////
// Main

int main()
{
	auto lines = AdventUtil::ReadFile(FILENAME);

	long sum = 0;
	for (const auto& line : lines)
		sum += SnafuToLong(line);

	std::cout << "Result: " << LongToSnafu(sum) << "\n";
	return 0;
}
