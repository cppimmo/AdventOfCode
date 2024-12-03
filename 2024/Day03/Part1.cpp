#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main() {
	const std::regex mulRegex(R"(mul\((\d{1,3}),(\d{1,3})\))");
	std::ostringstream oss;
	for (std::string line; std::getline(std::cin, line);) oss << line;
	std::string str = oss.str();

	std::cout << std::accumulate(
		std::sregex_iterator(str.cbegin(), str.cend(), mulRegex),
		std::sregex_iterator{}, 0, [](int acc, const std::smatch &sm) {
			int a = std::stoi(sm[1].str()), b = std::stoi(sm[2].str());
			return acc + (a * b);
		}) << '\n';
}
