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

/**
 ** Rucksack
 ** Even number of items.
 ** First half = first compartment
 ** Second half = second compartment
 **
 **/

class Rucksack
{
	friend std::ostream &operator<<(std::ostream &out, const Rucksack &rucksack);
public:
	Rucksack(const std::string &first, const std::string &second);
	const std::string &FirstCompartment(void) const;
	const std::string &SecondCompartment(void) const;
public:
	static Rucksack Create(const std::string &compartments);
	static std::optional<char> FindSharedItem(const Rucksack &rucksack);
	static int ItemPriority(char item);
private:
	std::pair<std::string, std::string> contents;
};
using RucksackVector = std::vector<Rucksack>;

RucksackVector CollectRucksacks(std::ifstream &input);
std::vector<int> CalculateSharedItemPriorities(const RucksackVector &rucksacks);

int
main(int argv, char *argc[])
{
	try {
		std::ifstream input("input.txt");
		if (!input.is_open())
			return -1;

		const auto rucksacks = CollectRucksacks(input);
		const auto priorities = CalculateSharedItemPriorities(rucksacks);
		std::cout << "Priority sum: " << std::accumulate(
			priorities.begin(), priorities.end(), 0) << '\n';

		const auto result = Rucksack::FindSharedItem(Rucksack::Create("ZnLdjRQddLRnZrlScHRVTTSHhRvg"));
		if (!result)
			return 1;
		std::cout << "\n\n" << result.value() << '\n';
		return 0;
	}
	catch (const std::exception &except) {
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

std::ostream &
operator<<(std::ostream &out, const Rucksack &rucksack)
{
	out << rucksack.FirstCompartment() << "|" << rucksack.SecondCompartment();
	return out;
}

Rucksack::Rucksack(const std::string &first, const std::string &second)
	: contents{ std::make_pair(first, second) }
{
}

const std::string &
Rucksack::FirstCompartment(void) const
{
	return this->contents.first;
}

const std::string &
Rucksack::SecondCompartment(void) const
{
	return this->contents.second;
}

Rucksack
Rucksack::Create(const std::string &compartments)
{
	const auto length = compartments.size();
	if ((length % 2) != 0)
		throw std::runtime_error("Input length is uneven!");
	const auto midpoint = length / 2;
	return Rucksack{
		compartments.substr(0, midpoint),
		compartments.substr(midpoint)
	};
}

int
Rucksack::ItemPriority(char item)
{
	if (!std::isalpha(item))
		throw std::runtime_error("Item is not alphabetic!");
	return (std::islower(item))
		? (static_cast<int>(item) - 96) : (static_cast<int>(item) - 38);
}

std::optional<char>
Rucksack::FindSharedItem(const Rucksack &rucksack)
{
	std::string shared{};
	{
		auto first = rucksack.FirstCompartment();
		std::ranges::sort(first);
		auto second = rucksack.SecondCompartment();
		std::ranges::sort(second);
		std::ranges::set_intersection(first, second, std::back_inserter(shared));
	}
	if (!(shared.size() >= 1))
		return std::nullopt;
	return shared[0];
}

RucksackVector
CollectRucksacks(std::ifstream &input)
{   /**
	 ** Place the input data into a list of rounds.
	 **/
	RucksackVector results{};
	for (std::string line; std::getline(input, line); ) {
		results.push_back(Rucksack::Create(line));
		std::cout << results.back() << '\n';
	}
	return results;
}

std::vector<int>
CalculateSharedItemPriorities(const RucksackVector &rucksacks)
{   /**
	 **
	 **/
	std::vector<int> priorities{};
	for (const Rucksack &rucksack : rucksacks) {
		const auto sharedItem = Rucksack::FindSharedItem(rucksack);
		if (!sharedItem) { // Shared item not found.
			std::cerr << "Shared item not found!\n";
			continue;
		}
		std::cout << '\n' << sharedItem.value() << '\n';
		priorities.push_back(Rucksack::ItemPriority(sharedItem.value()));
	}
	return priorities;
}
