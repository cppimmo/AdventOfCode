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

/**
 ** Opponent:
 ** A: Rock, B: Paper, & C: Scissors
 ** Me:
 ** X: Rock, Y: Paper, & Z: Scissors
 **
 ** My total score = sum of my scores for each round
 ** Round score:
 ** Shape selected (Rock 1, Paper 2, Scissors 3)
 ** plus
 ** Outcome score (Lost 0, Draw 3, 6 Won)
 **/

enum struct Shape
{
	Rock = 0,
	Paper,
	Scissors
};
// Pair type to store shapes played in a round.
using Round = std::pair<Shape, Shape>;

Shape TranslateShape(char shape);
int SumScores(const std::vector<int> &scores);
std::vector<Round> CollectRounds(std::ifstream &input);
std::vector<int> CalculateScores(const std::vector<Round> &rounds);

int
main(int argv, char *argc[])
{
	try {
		std::ifstream input("input.txt");
		if (!input.is_open())
			return -1;
		const auto rounds = CollectRounds(input);
		// My scores for each rounds:
		const auto scores = CalculateScores(rounds);
		std::cout << "My total score: " << SumScores(scores) << '\n';
		return 0;
	}
	catch (const std::exception &except) {
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

Shape
TranslateShape(char shape)
{   /**
	 ** Translate to Shape enumeration based on single character input.
	 **/
	using enum Shape;
	switch (shape) {
	case 'A':
	case 'X':
		return Rock;
		break;
	case 'B':
	case 'Y':
		return Paper;
		break;
	case 'C':
	case 'Z':
		return Scissors;
		break;
	default:
		throw std::runtime_error("Error shape!");
	}
	return {};
}

int
SumScores(const std::vector<int> &scores)
{
	return std::accumulate(
		scores.cbegin(), scores.cend(),
		0); // Initial value
}

std::vector<Round>
CollectRounds(std::ifstream &input)
{   /**
	 ** Place the input data into a list of rounds.
	 **/
	std::vector<Round> results{};
	for (std::string line; std::getline(input, line); ) {	
		Shape opponent{}, me{};
		if (line.size() >= 3) {
			opponent = TranslateShape(line[0]);
			me = TranslateShape(line[2]);
		}
		else
			continue;
		results.push_back(std::make_pair(opponent, me));
	}
	return results;
}

std::vector<int>
CalculateScores(const std::vector<Round> &rounds)
{   /**
	 **
	 **/
	auto scoreShape = [](Shape selection) -> int {
		using enum Shape;
		switch (selection) {
		case Rock:
			return 1;
		case Paper:
			return 2;
		case Scissors:
			return 3;
		}
		return {};
	};
	auto scoreOutcome = [](const Round &round) {
		using enum Shape;
		if (round.first == round.second) {
			return 3; // Draw
		}
		else if (
			(round.second == Rock     && round.first == Scissors)
		 || (round.second == Scissors && round.first == Paper)
		 || (round.second == Paper    && round.first == Rock)) {
			return 6; // Win
		}
		else
			return 0; // Lose
	};
	
	std::vector<int> scores{};
	for (const Round &round : rounds)
		scores.push_back(scoreShape(round.second) + scoreOutcome(round));
	return scores;
}
