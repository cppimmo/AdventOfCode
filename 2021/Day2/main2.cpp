#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>

enum struct MovementEvent {
	Forward, // Horizontal position.
	Down, // Depth control down.
	Up // Depth control up.
};

struct Submarine {
	int horizontal;
	int vertical; // Depth.
	int aim;
};

static MovementEvent StringToMovementEvent(std::string_view);
static int PositionProduct(const Submarine &);
static Submarine DoMovement(const Submarine &, MovementEvent, int);
static std::ostream &operator<<(std::ostream &, const Submarine &);

int
main(int argc, char *argv[])
{
	Submarine sub = {
		.horizontal = 0,
		.vertical = 0,
		.aim = 0
	};
	std::ifstream inputFile;
	inputFile.open("input.txt"); // input_test.txt
	if (!inputFile.is_open())
		return 1;
	std::string line{};
	while (inputFile.peek() != EOF && std::getline(inputFile, line)) {
		const auto index = line.find_first_of(' ', 0);
		if (index == std::string::npos) {
			std::cerr << "Error: There should be a space!\n";
			break;
		}
		const auto directionString = line.substr(0, index);
		const auto direction = StringToMovementEvent(directionString);
		const auto delta = std::stoi(line.substr(index + 1, line.size()));
		sub = DoMovement(sub, direction, delta);
		std::cout << directionString << ':' << delta << '\n';
		std::cout << '\n' << sub << '\n';
	}
	std::cout << "\nFinal Submarine Position: " << sub << '\n';
	std::cout << "\nPosition Product of Submarine: "
			  << PositionProduct(sub) << '\n';
	return 0;
}

static MovementEvent
StringToMovementEvent(std::string_view string)
{
	using namespace std::string_view_literals;
	using enum MovementEvent;
	if (string == "forward"sv)
		return { Forward };
	else if (string == "down"sv)
		return { Down };
	else if (string == "up"sv)
		return { Up };
	else
		assert(false);
	return { Forward };
}

static int
PositionProduct(const Submarine &sub)
{
	return { sub.horizontal * sub.vertical };
}


static Submarine
DoMovement(const Submarine &currentSub, MovementEvent event, int delta)
{
	using enum MovementEvent;
	switch (event) {
	case Forward:
		return {
			.horizontal = currentSub.horizontal + delta,
			.vertical = currentSub.vertical + (currentSub.aim * delta),
			.aim = currentSub.aim
		};
		break;
	case Down:
		return {
			.horizontal = currentSub.horizontal,
			.vertical = currentSub.vertical,
			.aim = currentSub.aim + delta
		};
		break;
	case Up:
		return {
			.horizontal = currentSub.horizontal,
			.vertical = currentSub.vertical,
			.aim = currentSub.aim - delta
		};
		break;
	default:
		assert(false);
	}
	return currentSub;
}

static std::ostream &
operator<<(std::ostream &os, const Submarine &sub)
{
	os << sub.horizontal << ':' << sub.vertical << ':' << sub.aim;
	return os;
}

