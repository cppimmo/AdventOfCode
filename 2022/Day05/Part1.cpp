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
#include <stack>
#include <memory>
#include <regex>
#include <type_traits>

using CrateStack = std::stack<char>;
using CrateStackVector = std::vector<CrateStack>;

class Instruction;
using InstructionVector = std::vector<Instruction>;

class Instruction
{
	friend std::ostream &operator<<(std::ostream &out,
									const Instruction &instruction);
public:
	Instruction(std::size_t amount, std::size_t startIndex,
				std::size_t endIndex);
	std::size_t GetAmount(void) const;
	std::size_t GetStartIndex(void) const;
	std::size_t GetEndIndex(void) const;
public:
	static Instruction Create(std::string_view string);
	static void Perform(CrateStackVector &stacks,
						const InstructionVector &instructions);
private:
	std::size_t amount, startIndex, endIndex;
};

std::ostream &operator<<(std::ostream &out, const CrateStackVector &stacks);
template <typename Type>
void ReverseStack(std::stack<Type> &stack);
CrateStackVector CollectStacks(std::ifstream &input);
InstructionVector CollectInstructions(std::ifstream &input);
std::string GetTopCrates(const CrateStackVector &stacks);

int
main(int argv, char *argc[])
{
	try {
		std::ifstream input("input.txt");
		if (!input.is_open())
			throw std::runtime_error("Input file not found!");

		auto stacks = CollectStacks(input);
		//input.seekg(0); // Return istream position to beginning of file.
		auto instructions = CollectInstructions(input);
		// std::cout << "Number of instructions: " << instructions.size() << '\n';
		//for (const auto instruction : instructions)
		//	std::cout << instruction << '\n';
		Instruction::Perform(stacks, instructions);
		std::cout << stacks;
		std::cout << "Answer: " << GetTopCrates(stacks) << '\n';
		return 0;
	}
	catch (const std::exception &except) {
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

std::ostream &
operator<<(std::ostream &out, const Instruction &instruction)
{
	out << instruction.amount << ' ' << instruction.startIndex << ' '
		<< instruction.endIndex;
	return out;
}

Instruction::Instruction(
	std::size_t amount, std::size_t startIndex,
	std::size_t endIndex)
	: amount(amount), startIndex(startIndex), endIndex(endIndex)
{
}

std::size_t
Instruction::GetAmount(void) const
{
	return this->amount;
}

std::size_t
Instruction::GetStartIndex(void) const
{
	return this->startIndex;
}

std::size_t
Instruction::GetEndIndex(void) const
{
	return this->endIndex;
}

Instruction
Instruction::Create(std::string_view string)
{
	std::unique_ptr<std::istringstream> iss{ nullptr };
	{
		std::string temp(string);
		temp.erase(std::remove_if(temp.begin(), temp.end(),
								  [](unsigned char character) -> bool {
									  return std::isalpha(character);
								  }),
				   temp.end());
		iss = std::make_unique<decltype(iss)::element_type>(temp);
	}
	std::size_t amount{}, startIndex{}, endIndex{};
	*(iss) >> amount >> startIndex >> endIndex;
	if (iss->fail())
		throw std::runtime_error("Input stream failure!");
	return Instruction(amount, startIndex, endIndex);
}

void
Instruction::Perform(CrateStackVector &stacks,
					 const InstructionVector &instructions)
{
	for (auto &instruction : instructions) {
		for (std::size_t count{ 0 };
			 count < instruction.GetAmount(); ++count) {
			const auto startIndex{ instruction.GetStartIndex() - 1 },
				endIndex{ instruction.GetEndIndex() - 1 };
			const auto crate = stacks[startIndex].top();
			stacks[startIndex].pop();
			stacks[endIndex].push(crate);
		}
	}
}

std::ostream &
operator<<(std::ostream &out, const CrateStackVector &stacks)
{
	for (const auto &stack : stacks) {
		auto tempStack = stack;
		while (!tempStack.empty()) {
			out << '[' << tempStack.top() << "] ";
			tempStack.pop();
		}
		out << '\n';
	}
	return out;
}

template <typename Type>
void ReverseStack(std::stack<Type> &stack)
{
	std::remove_reference_t<decltype(stack)> tempStack;
	while (!stack.empty()) {
		auto element = stack.top();
		stack.pop();
		tempStack.push(element);
	}
	stack = tempStack;
}

CrateStackVector
CollectStacks(std::ifstream &input)
{   /**
	 ** Place the input data into a list of section range pairs.
	 **/	
	decltype(CollectStacks(input)) results{};
	std::string line;
	bool isDone{ false };
	for (; !isDone && std::getline(input, line); ) {
		for (std::size_t index{ 0 }; index < line.length() - 1; ++index) {
			if (std::isdigit(line[index])) {
				isDone = true;
				break;
			}
			if (!std::isupper(line[index]))
				continue;
			
			std::size_t stackIndex{ (index - 1) /  4 };
			std::cout << "Index: " << index << ", StackIndex: "
					  << stackIndex << ", Crate: " << line[index] << '\n';
			if (results.size() <= stackIndex)
				results.resize(stackIndex + 1);
			results[stackIndex].push(line[index]);
		}
	}
	// Reverse the direction of each stack.
	std::for_each(results.begin(), results.end(),
				  [](auto &stack) -> void {
					  ReverseStack(stack);
				  });
	std::getline(input, line);
	return results;
}

InstructionVector
CollectInstructions(std::ifstream &input)
{   /**
	 ** Place the input data into a list of section range pairs.
	 **/	
	decltype(CollectInstructions(input)) results{};
	//bool hasReachedEmptyLine{ false };
	for (std::string line; std::getline(input, line); ) {
		//if (line.empty() && !hasReachedEmptyLine) {
		//		hasReachedEmptyLine = true;
//			continue;
//		} else {
			// Wait until we reach the empty line in our input file.
//			if (hasReachedEmptyLine) {
				results.push_back(Instruction::Create(line));
				
//			}
				// }
	}
	return results;
}

std::string
GetTopCrates(const CrateStackVector &stacks)
{   /**
	 ** This function gets the answer which is a string with the identifers
	 ** of the crates that rests of the top of each stack.
	 **/
	std::string result{};
	for (const auto &stack : stacks)
		result.append(1, stack.top()); // Append crate characters.
	return result;
}
