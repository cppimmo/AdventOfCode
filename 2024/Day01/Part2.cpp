#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main(void)
{
	std::vector<int> lefts, rights;
	int l, r;
	while (std::cin >> l >> r)
		lefts.push_back(l), rights.push_back(r);

	std::cout << std::accumulate(lefts.begin(), lefts.end(), 0,
		[&rights](int acc, int l) { return acc + l * std::ranges::count(rights, l); }) << '\n';
	return 0;
}
