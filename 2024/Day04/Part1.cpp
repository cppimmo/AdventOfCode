#include "AdventOfCodeStd.hpp"

using namespace AOC;

auto count_occurences(std::string_view str, std::string_view substr) -> int {
	int count = 0;
    std::size_t pos = str.find(substr);

    while (pos != std::string_view::npos) {
        ++count;
        pos = str.find(substr, pos + substr.size());
    }
    return count;
}

int main() {
	std::vector<std::string> lines;
	for (std::string line; std::getline(std::cin, line);) lines.push_back(line);
	assert(!lines.empty());

	constexpr std::string_view token = "XMAS";
	constexpr std::string_view rtoken = "SAMX";
	const std::size_t rows = lines.size(), cols = lines[0].size();
	int total = 0;
	// Count horizontal lines
	for (const auto &line : lines) {
		total += count_occurences(line, token);
		total += count_occurences(line, rtoken);
	}
	// Count vertical lines (column major)
	for (std::size_t col = 0; col < cols; ++col) {
		std::string column;
		for (std::size_t row = 0; row < rows; ++row)
			column += lines[row][col];
		total += count_occurences(column, token);
		total += count_occurences(column, rtoken);
	}

	for (std::size_t start = 0; start < rows + cols - 1; ++start) {
		std::string diag1, diag2;
		for (std::size_t row = 0; row < rows; ++row) {
			int col1 = start - row;
			int col2 = cols - 1 - start + row;
			if (col1 >= 0 && col1 < cols) diag1 += lines[row][col1];
			if (col2 >= 0 && col2 < cols) diag2 += lines[row][col2];
		}
		total += count_occurences(diag1, token);
		total += count_occurences(diag1, rtoken);
		total += count_occurences(diag2, token);
		total += count_occurences(diag2, rtoken);
	}
	std::cout << total << '\n';
}
