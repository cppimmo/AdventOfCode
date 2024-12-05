#include "AdventOfCodeStd.hpp"

using namespace AOC;

auto print(auto &&container) {
	for (const auto &elem : container)
		std::cout << elem << ' ';
}

auto parse_rules() -> std::vector<std::pair<int, int>> {
	std::vector<std::pair<int, int>> rules;
	for (std::string line; std::getline(std::cin, line) && !line.empty();) {
		std::ranges::transform(line, line.begin(), [](auto ch) { return std::isdigit(ch) ? ch : ' '; });
		std::istringstream iss(line);
		std::pair<int, int> rule;
		iss >> rule.first >> rule.second;
		rules.emplace_back(rule);
	}
	return rules;
}

auto parse_updates() -> std::vector<std::vector<int>> {
	std::vector<std::vector<int>> updates;
	for (std::string line; std::getline(std::cin, line);) {
		std::vector<int> pages;
		std::istringstream iss(line);
		for (std::string token; std::getline(iss, token, ',');) {
			pages.push_back(std::stoi(token));
		}
		updates.emplace_back(pages);
	}
	return updates;
}

auto valid_update(const std::vector<int> &pages, auto &&rules) -> bool {
	std::unordered_map<std::remove_reference_t<decltype(pages)>::value_type, std::size_t> indexes;
	for (std::size_t i = 0; i < pages.size(); ++i)
		indexes[pages[i]] = i;

	for (const auto &rule : rules) {
		int x = rule.first, y = rule.second;
		if (indexes.contains(x) && indexes.contains(y)) {
			if (indexes[x] >= indexes[y]) return false;
		}
	}
	return true;
}

auto main() -> int {
	auto rules = parse_rules();
	auto updates = parse_updates(); // Row in updates is termed "pages"

	for (const auto &rule : rules)
		std::cout << rule.first << ' ' << rule.second << '\n';

	for (const auto &pages : updates) {
		print(pages);
		std::cout << '\n';
	}

	const int median_sum = std::accumulate(
		updates.cbegin(), updates.cend(), 0,
		[&](int acc, const auto &pages) {
			// If pages are sorted
			if (valid_update(pages, rules)) {
				const auto it = pages.cbegin() + (pages.size() / 2);
				acc += *it;
			}
			return acc;
		}
	);
	std::cout << median_sum << '\n';
}
