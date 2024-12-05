#include "AdventOfCodeStd.hpp"

using namespace AOC;

auto count_xmas(const std::vector<std::string> &grid) -> int {
    constexpr std::string_view token = "MAS";
    constexpr std::string_view rtoken = "SAM";

    const std::size_t rows = grid.size();
    const std::size_t cols = grid[0].size();
    int total = 0;

    return total;
}

int main() {
	std::vector<std::string> lines;
	for (std::string line; std::getline(std::cin, line);) lines.push_back(line);
	assert(!lines.empty());

	const int total = count_xmas(lines);
	std::cout << total << '\n';
}
