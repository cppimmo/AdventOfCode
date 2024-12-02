#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main(void) {
	std::vector<std::vector<int>> reports;
	for (std::string line; std::getline(std::cin, line);) {
		std::istringstream iss(line);
		reports.emplace_back(std::istream_iterator<int>(iss), std::istream_iterator<int>{});
	}

	std::cout << std::ranges::count_if(reports, [](const auto &report) {
			return std::ranges::adjacent_find(report, [](int a, int b) {
				const int diff = std::abs(a - b);
				return diff < 1 || diff > 3;
			}) == report.cend() &&
			(std::ranges::is_sorted(report) || std::ranges::is_sorted(report, std::greater{}));
		}) << '\n';
	return 0;
}
