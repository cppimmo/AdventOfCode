#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <numeric>

// set values to zero
inline static void DefaultArray(std::array<int, 3> &arr)
{
	for (auto &value : arr)
		value = 0;
}

inline static int SumArray(std::array<int, 3> &arr)
{
	return std::accumulate(arr.cbegin(), arr.cend(), 0);
}

int main(int argc, char *argv[])
{
	const std::string filename = "input2.txt";
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "File failed to open!\n";
		return 1;
	}

	/* occurences is the number of times a number was greater
	   than the last entry */
	std::size_t iter = 0,
		totaliter = 0,
		increased = 0;
	int lastsum = 0;
	std::array<int, 3> pair;
	std::vector<int> content;
	for (std::string line; std::getline(file, line);)
	{
		const int value = std::stoi(line);
		content.push_back(value);
	}
	pair[0] = content[0];
	pair[1] = content[1];
	pair[2] = content[2];
	for (std::size_t i = 0; i < content.size(); i += 3)
	{
	    DefaultArray(pair);

		if (i + 3 < content.size())
		{
			pair[i] = content[i];
			pair[i + 1] = content[i + 1];
			pair[i + 2] = content[i + 2];
		}

		const auto currsum = SumArray(pair);
		if (currsum > lastsum)
			increased += 1;

		lastsum = currsum;
		std::cout << content[i] << ':' << iter << '\n';
	}
	std::cout << "Answer (# of increases in sum): " << increased << std::endl;
	return 0;
}
/* Answer: 1665 */
