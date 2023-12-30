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
#include <variant>
#include <unordered_map>
#include <sstream>
#include <deque>

static void FilterByTotalSize(std::unordered_map<std::string, int> &dirSizes);
static int ProcessShellSession(std::ifstream &input);

int main(int argv, char *argc[])
{
	try
	{
		std::ifstream input("input.txt");
		if (!input.is_open())
			throw std::runtime_error("Input file not found!");

		auto result = ProcessShellSession(input);
		std::cout << "Total sum of the size of directories up to 100000 bytes: " << result << '\n';

		std::cin.get();
		return 0;
	}
	catch (const std::exception &except)
	{
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

void FilterByTotalSize(std::unordered_map<std::string, int> &dirSizes)
{
	std::erase_if(dirSizes,
				  [](const auto &elem)
				  {
					  const auto &[key, value] = elem;
					  return (value > 100'000);
				  });
}

int ProcessShellSession(std::ifstream &input)
{
	std::unordered_map<std::string, int> dirSizes;
	std::deque<std::string> dirTree;
	std::string currentPath = "";

	for (std::string line; std::getline(input, line); )
	{
		auto find = [&line](auto tokenString) -> bool
		{
			return (line.find(tokenString) != std::string::npos);
		};

		bool isChDir = find("$ cd"),
			 isChDirOut = find("$ cd .."),
			 isChDirRoot = find("$ cd /");

		if (isChDir)
		{
			std::cout << "Current path: " << currentPath << '\n';
			if (!isChDirOut && !isChDirRoot)
			{
				std::istringstream iStrStream(line);
				std::string temp, argDir;

				iStrStream >> temp >> temp >> argDir;
				std::cout << "Change to explicitly named directory ( Arg dir: " << argDir << " )\n";

				if (currentPath == "/")
				{
					currentPath += argDir;
				}
				else
				{
					currentPath += std::string("/") + argDir;
				}
				std::cout << "Adding ( " << currentPath << " ) to dir tree.\n";
				dirTree.push_back(currentPath);
			}
			else if (isChDirRoot)
			{
				std::cout << "Change to root directory\n";
				
				currentPath = "/";
				dirTree.clear();
				dirTree.push_back("/");
			}
			else if (isChDirOut)
			{
				std::string oldPath = currentPath;
				currentPath = currentPath.substr(0, currentPath.find_last_of('/'));
				std::cout << "Change to one level out ( Old: " << oldPath << ", Curr: " << currentPath << " )\n";

				dirTree.pop_back();
			}
		}
		else if (std::isdigit(line[0]))
		{
			int fileSize = 0;
			std::istringstream iStrStream(line);

			iStrStream >> fileSize;
			std::cout << "File size = " << fileSize << '\n';

			for (auto &dir : dirTree)
			{
				dirSizes[dir] += fileSize;
			}
		}
	}
	
	// Remove all directory trees with a total size of more than 100,000 bytes.
	FilterByTotalSize(dirSizes);
	// Sum the newly filtered sizes.
	std::vector<int> filteredSizes;

	for (const auto &[key, value] : dirSizes)
	{
		filteredSizes.push_back(value);
	}

	return std::accumulate(filteredSizes.cbegin(), filteredSizes.cend(), 0);
}