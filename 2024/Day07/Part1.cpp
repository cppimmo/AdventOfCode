#include "AdventOfCodeStd.hpp"

using namespace AOC;

using Equation = std::pair<std::int64_t, std::vector<std::int64_t>>;

enum Operator : int
{
	kOPERATOR_ADD,
	kOPERATOR_MULTIPLY
};

int main(void)
{
	std::vector<Equation> equations;
	for (std::string line; std::getline(std::cin, line);)
	{
		const auto idx = line.find(':');
		const auto result = std::stoll(line.substr(0, idx));

		std::istringstream iss(line.substr(idx + 1));

		std::vector<std::int64_t> values;
		for (std::int64_t n; iss >> n;)
			values.push_back(n);

		equations.emplace_back(result, values);
	}

	for (const auto &kEquation : equations)
	{
		std::cout << kEquation.first << ": ";
		for (const auto n : kEquation.second)
			std::cout << n << ' ';
		std::cout << '\n';
	}

	std::int64_t total = 0;
	const auto kCount = std::ranges::count_if(equations, [&total](const auto &kEquation)
		{
			const auto &[kResult, kValues] = kEquation;

			// Need at least 2 values for binary operation
			if (kValues.size() < 2) return false;

			const std::size_t kOperatorCount = kValues.size() - 1;
			std::vector<Operator> operators(kOperatorCount, kOPERATOR_ADD);
			operators.insert(operators.end(), kOperatorCount, kOPERATOR_MULTIPLY);
			std::ranges::sort(operators);

			do
			{
				// Calculate result with the current operator permutation.
				std::int64_t result = kValues[0];
				for (std::size_t i = 0; i < kOperatorCount; ++i)
				{
					if (operators[i] == kOPERATOR_ADD)
						result += kValues[i + 1];
					else
						result *= kValues[i + 1];
				}

				// Check if the result matches the target.
				if (result == kResult)
				{
					total += kResult;
					return true;
				}
			}
			while (std::ranges::next_permutation(operators).found);
			return false;
		}
	);
	std::cout << total << '\n';
	return 0;
}
