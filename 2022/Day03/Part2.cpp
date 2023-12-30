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
constexpr std::size_t groupSize{ 3 };
class Rucksack
{
	friend std::ostream &operator<<(std::ostream &out, const Rucksack &rucksack);
public:
	Rucksack(void) = default;
	Rucksack(const std::string &first, const std::string &second);
	const std::string &FirstCompartment(void) const;
	const std::string &SecondCompartment(void) const;
public:
	static Rucksack Create(const std::string &compartments);
	static std::optional<char> FindSharedItem(const Rucksack &rucksack);
	static std::optional<char> FindSharedItemGroup(
		const std::array<Rucksack, groupSize> &group);
	static int ItemPriority(char item);
private:
	std::pair<std::string, std::string> contents;
};
using RucksackVector = std::vector<std::array<Rucksack, 3>>;

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
		std::cout << "Priority sum: "
				  << std::accumulate(priorities.begin(), priorities.end(),
									 0) << '\n';
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

std::optional<char>
Rucksack::FindSharedItemGroup(const std::array<Rucksack, groupSize> &group)
{
	std::string shared{};
	std::array<std::string, groupSize> sharedItems;
	for (std::size_t index{ 0 }; index < groupSize; ++index) {
		std::string shared{};
		{
			auto first = group[index].FirstCompartment();
			std::ranges::sort(first);
			auto second = group[index].SecondCompartment();
			std::ranges::sort(second);
			std::ranges::set_intersection(first, second, std::back_inserter(shared));
		}
		sharedItems[index] = shared;
		std::cout << "Shared[" << index << "] = " << shared << '\n';
	}
	return 'a';
}

RucksackVector
CollectRucksacks(std::ifstream &input)
{   /**
	 ** Place the input data into a list of rounds.
	 **/
	RucksackVector results{};
	std::string line{};
	std::array<std::string, groupSize> lineArray{};
	std::size_t groupIndex{}; // Index of elf in group.
	for (groupIndex = 0; std::getline(input, line); ) {
		lineArray[groupIndex] = line;
		if (groupIndex <= groupSize)
			groupIndex += 1;
		else {
			std::array<Rucksack, 3> group;
			for (std::size_t index = 0; index < groupSize; ++index) {
				group[index] = Rucksack::Create(lineArray[index]);
			}
			results.push_back(group);
			groupIndex = 0;
			std::cout << '\n';
			for (const Rucksack &rucksack : results.back())
				std::cout << rucksack << '\n';
		}
	}
	return results;
}

std::vector<int>
CalculateSharedItemPriorities(const RucksackVector &rucksacks)
{   /**
	 **
	 **/
	std::vector<int> priorities{};
    for (const auto &rucksackGroup : rucksacks) {
		const auto sharedItem = Rucksack::FindSharedItemGroup(rucksackGroup);
		//for (const Rucksack &rucksack : rucksackGroup) {
			//const auto sharedItem = Rucksack::FindSharedItem(rucksack);
			//if (!sharedItem) { // Shared item not found.
				//std::cerr << "Shared item not found!\n";
				//continue;
				//}
			//std::cout << '\n' << sharedItem.value() << '\n';
			//priorities.push_back(Rucksack::ItemPriority(sharedItem.value()));
		//}
	}
	return priorities;
}
