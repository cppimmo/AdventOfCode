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

class Elf
{
public:
	Elf(void);
	Elf(int totalCalories);
	void PushCalorie(int calorie);
	void ClearCalories(void);
	int TotalCalories(void) const;
	Elf operator+(const Elf &rhs) const;
private:
	std::vector<int> itemCalories;
};

std::vector<Elf> Collect(std::ifstream &input);
std::array<Elf, 3> Operate(const std::vector<Elf> &elfs);

int
main(int argv, char *argc[])
{
	std::ifstream input("input.txt");
	if (!input.is_open())
		return -1;
	const auto maxElfs = Operate(Collect(input));
	std::cout << "Calories possessed by top three Elfs: "
			  << std::accumulate(
				  maxElfs.cbegin(), maxElfs.cend(),
				  Elf{}).TotalCalories() << '\n';
	return 0;
}

Elf::Elf(void)
	: itemCalories(3)
{ // Initialize with a reserve of three.
}

Elf::Elf(int totalCalories)
	: itemCalories({ totalCalories })
{
}

void
Elf::PushCalorie(int calorie)
{
	this->itemCalories.push_back(calorie);
}

void
Elf::ClearCalories(void)
{
	this->itemCalories.clear();
}

int
Elf::TotalCalories(void) const
{
	return std::accumulate(itemCalories.cbegin(), itemCalories.cend(), 0);
}

Elf
Elf::operator+(const Elf &rhs) const
{
	return this->TotalCalories() + rhs.TotalCalories();
}

std::vector<Elf>
Collect(std::ifstream &input)
{   /**
	 ** Place the input data into a list of Elfs.
	 **/
	std::vector<Elf> result{};
	Elf temp{};
	int calorieNumber{ 0 };
	for (std::string line; std::getline(input, line); ) {
		try {
			calorieNumber = std::stoi(line);
		}
		catch (const std::invalid_argument &except) {
			std::cerr << "Blank line!\n";
			result.push_back(temp);
			temp.ClearCalories();
			continue;
		}
		temp.PushCalorie(calorieNumber);
		std::cout << "Calorie number = " << calorieNumber << '\n';
	}
	return result;
}

std::array<Elf, 3>
Operate(const std::vector<Elf> &elfs)
{   /**
	 ** Return the Elf in the list with the most calories.
	 **/
	auto tempElfs(elfs);
	std::sort(
		tempElfs.begin(), tempElfs.end(),
		[](const Elf &lhs, const Elf &rhs) -> bool {
			return lhs.TotalCalories() < rhs.TotalCalories();
		});
	return {
		*(tempElfs.cend() - 1),
		*(tempElfs.cend() - 2),
		*(tempElfs.cend() - 3)
	};
}
