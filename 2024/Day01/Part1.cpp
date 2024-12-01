#include "AdventOfCodeStd.hpp"

using namespace AOC;

int main(void)
{
	std::multiset<int> lefts, rights;
	int l, r;
	while (std::cin >> l >> r)
		lefts.insert(l), rights.insert(r);

	std::cout << std::transform_reduce(lefts.begin(), lefts.end(), rights.begin(), 0,
		std::plus{}, [](int l, int r) { return std::abs(l - r); }) << '\n';
	return 0;
}
