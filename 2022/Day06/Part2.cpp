#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstdio>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <string>
#include <utility>
#include <exception>
#include <cctype>
#include <stack>
#include <memory>
#include <regex>
#include <type_traits>

//CrateStackVector CollectStacks(std::ifstream &input);

int
main(int argv, char *argc[])
{
	try {
		std::ifstream input("input.txt");
		if (!input.is_open())
			throw std::runtime_error("Input file not found!");

		std::string str{};
		std::getline(input, str);
		int numProcessed{ 13 };
		for (auto iter = str.begin() + 13; iter != str.end(); ++iter) {
			numProcessed += 1;
			std::string unsorted;
			std::string temp = unsorted = std::string(iter - 13, iter + 1);
			std::ranges::sort(temp);
			const auto resultIter = std::ranges::adjacent_find(temp);
			if (resultIter != temp.end())
				continue; // Duplicate characters.
			else {
				std::cout << unsorted << ' ' << numProcessed  << '\n';
				break;
			}
		}
		//auto stacks = CollectStacks(input);
		return 0;
	}
	catch (const std::exception &except) {
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

/*CrateStackVector
CollectStacks(std::ifstream &input)
{
	decltype(CollectStacks(input)) results{};
	std::string line;
	for (; !isDone && std::getline(input, line); ) {
	}
	return results;
}*/
