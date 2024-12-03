#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main() {
	const std::regex mulRegex(R"(do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\))");
	std::ostringstream oss;
	for (std::string line; std::getline(std::cin, line);) oss << line;
	std::string str = oss.str();

	bool enabled = true;
	std::cout << std::accumulate(
		std::sregex_iterator(str.cbegin(), str.cend(), mulRegex),
		std::sregex_iterator{}, 0, [&enabled](int acc, const std::smatch &sm) mutable {
			std::string s = sm.str();
			bool tmp;
			if ((tmp = (s == "do()")) || s == "don't()") {
				enabled = tmp;
				return acc;
			}
			int a = std::stoi(sm[1].str()), b = std::stoi(sm[2].str());
			return acc + (enabled ? (a * b) : 0);
		}) << '\n';
}
