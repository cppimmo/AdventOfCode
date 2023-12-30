#include <iostream>
#include <fstream>
#include <string>
#include <vector>

inline void PrintNumbers(const int num1, const int num2, const int occurences)
{
	std::cout << "Current=" << num1 << ", Last=" << num2 <<
		", Occurences=" << occurences <<'\n';
}

int main(int argc, char *argv[])
{
	const std::string filename = "input1.txt";
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "File failed to open!\n";
		return 1;
	}
	int last = 0, current = 0, occurences = 0;
	/* occurences is the number of times a number was greater
	   than the last entry */
	std::size_t iter = 0;
	for (std::string line; std::getline(file, line); ++iter)
	{
		current = std::stoi(line);
		PrintNumbers(current, last, occurences);
		if (current > last && iter > 0)
			++occurences;
		last = current;
	}
	std::cout << "The total number of iterations: " << iter << '\n';
	std::cout << "The total number of occurences was: " << occurences << "\n";
	std::cout.flush();
	return 0;
}

/* Answer: 1665 */
