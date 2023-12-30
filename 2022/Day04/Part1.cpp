#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstdio>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <string>
#include <utility>
#include <exception>
#include <cctype>

using SectionRange = std::pair<int, int>;
using SectionPair  = std::pair<SectionRange, SectionRange>;
using SectionPairVector = std::vector<SectionPair>;

std::ostream &operator<<(std::ostream &out, const SectionRange &range);
std::ostream &operator<<(std::ostream &out, const SectionPair &pair);
SectionPair FlipSectionPair(const SectionPair &pair);
SectionRange TranslateSectionRange(const std::string &input);
bool IsSectionRangeMalformed(const SectionRange &range);
SectionPair  TranslateSectionPair(const std::string &input);
bool IsSectionPairMalformed(const SectionPair &pair);
SectionPairVector CollectSectionPairs(std::ifstream &input);
int CountSectionPairOverlaps(const SectionPairVector &pairs);
bool DoesSectionPairOverlap(const SectionPair &pair, bool isFullyContained = true);

int
main(int argv, char *argc[])
{
	try {
		std::ifstream input("input.txt");
		if (!input.is_open())
			throw std::runtime_error("Input file not found!");

		const auto sectionPairs = CollectSectionPairs(input);
		std::cout << "Total section pair overlaps: "
				  << CountSectionPairOverlaps(sectionPairs) << '\n';
		//const auto pair = TranslateSectionPair("2-8,3-7");
		//std::cout << pair.second.second << '\n';
		//std::cout << std::boolalpha << DoesSectionPairOverlap(pair, false) << '\n';
		return 0;
	}
	catch (const std::exception &except) {
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

std::ostream &
operator<<(std::ostream &out, const SectionRange &range)
{
	out << range.first << '-' << range.second;
	return out;
}

std::ostream &operator<<(std::ostream &out, const SectionPair &pair)
{
	out << pair.first << ',' << pair.second;
	return out;
}

SectionPair
FlipSectionPair(const SectionPair &pair)
{
	return { pair.second, pair.first };
}

SectionRange
TranslateSectionRange(const std::string &input)
{
	const auto index = input.find_first_of('-');
	if (index == std::string::npos)
		throw std::runtime_error("There should be a dash here!");		
	return std::make_pair(std::stoi(input.substr(0, index)),
						  std::stoi(input.substr(index + 1)));
}

bool
IsSectionRangeMalformed(const SectionRange &range)
{
	return !(range.first <= range.second);
}

SectionPair
TranslateSectionPair(const std::string &input)
{
	const auto index = input.find_first_of(',');
	if (index == std::string::npos)
		throw std::runtime_error("There should be a comma here!");
	return std::make_pair(
		TranslateSectionRange(input.substr(0, index)),
		TranslateSectionRange(input.substr(index + 1))
		);
}

bool
IsSectionPairMalformed(const SectionPair &pair)
{
	return (IsSectionRangeMalformed(pair.first)
			&& IsSectionRangeMalformed(pair.second));
}

SectionPairVector
CollectSectionPairs(std::ifstream &input)
{   /**
	 ** Place the input data into a list of section range pairs.
	 **/	
	decltype(CollectSectionPairs(input)) results{};
	for (std::string line; std::getline(input, line); ) {
		decltype(results)::value_type pair = TranslateSectionPair(line);
		results.push_back(pair);
		std::cout << results.back() << '\n';
	}
	return results;
}

int
CountSectionPairOverlaps(const SectionPairVector &pairs)
{
	int overlapTotal{ 0 };
	for (const auto &pair : pairs) {
		// Fully-contained overlap.
		if (DoesSectionPairOverlap(pair, true))
			overlapTotal += 1;
	}
	return overlapTotal;
}

bool
DoesSectionPairOverlap(const SectionPair &pair, bool isFullyContained)
{   /**
	 ** Test if SectionPair ranges fully contains or overlaps one another.
	 ** Test must be performed in both directions (i.e. flip the pair).
	 **/
	if (IsSectionPairMalformed(pair))
		throw std::runtime_error("Malformed section pair!");
	// Overlap procedure.
	const auto overlapRoutine = [isFullyContained](const SectionPair &pair)
		-> bool {
		const int &x1{ pair.first.first }, &x2{ pair.first.second };
		const int &y1{ pair.second.first }, &y2{ pair.second.second };
		return isFullyContained
			? (x1 >= y1 && x2 <= y2)  // Fully contained overlap.
			: (x1 <= y2 && y1 <= x2); // Non-fully-contained overlap. 
	};
	// Test for overlap in both directions.
	return (overlapRoutine(pair) || overlapRoutine(FlipSectionPair(pair)));
}
