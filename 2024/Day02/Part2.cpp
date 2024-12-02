#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main(void) {
	std::vector<std::vector<int>> reports;
	for (std::string line; std::getline(std::cin, line);) {
		std::istringstream iss(line);
		reports.emplace_back(std::istream_iterator<int>(iss), std::istream_iterator<int>{});
	}

	auto isSafe = [](const auto &report) {
		return std::ranges::adjacent_find(report, [](int a, int b) {
				const int diff = std::abs(a - b);
				return diff < 1 || diff > 3;
			}) == report.cend() &&
			(std::ranges::is_sorted(report) || std::ranges::is_sorted(report, std::greater{}));
	};
	std::cout << std::ranges::count_if(reports, [&isSafe](const auto &report) {
			std::vector<int> excludes(report.size());
			std::iota(excludes.begin(), excludes.end(), 0);
			return isSafe(report) || std::ranges::any_of(excludes, [&](int index) {
				auto copy = report;
				copy.erase(copy.begin() + index);
				return isSafe(copy);
			});
		}) << '\n';
	return 0;
}
