#include "AdventOfCodeStd.hpp"

using namespace AOC;

using Map = std::vector<std::vector<char>>;
using Position = std::pair<int, int>;

enum struct Direction
{
	kUp,
	kDown,
	kLeft,
	kRight
};

static std::ostream &operator<<(std::ostream &os, const Position &kPos)
{
	os << kPos.first << ' ' << kPos.second;
	return os;
}

static bool IsObstacle(char ch)
{
	return ch == '#';
}

static Direction ChangeDirection(Direction dir)
{
	using enum Direction;

	switch (dir)
	{
	case kUp:
		return kRight;
	case kDown:
		return kLeft;
	case kLeft:
		return kUp;
	case kRight:
		return kDown;
	default:
		return {};
	}
}

static char GetAdjacent(const Position &kPos, Direction dir, const Map &kMap)
{
	using enum Direction;
    // Define the offsets for each direction
    int dRow = 0, dCol = 0;
    switch (dir)
	{
	case kUp:
		dRow = -1; dCol = 0;
		break;
	case kDown:
		dRow = 1; dCol = 0;
		break;
	case kLeft:
		dRow = 0; dCol = -1;
		break;
	case kRight:
		dRow = 0; dCol = 1;
		break;
	default:
		return '.'; // Invalid direction
    }

    // Calculate the neighbor's position
    int newRow = kPos.first + dRow;
    int newCol = kPos.second + dCol;

    // Check bounds and return the character if valid
    if (newRow >= 0 && newRow < kMap.size() && newCol >= 0 && newCol < kMap[newRow].size())
	{
        return kMap[newRow][newCol];
    }

    // Return '.' if out of bounds
    return '.';
}

// Check that the position is in the bounds of the 2D map grid.
static bool InBounds(const Position &kPos, const Map &kMap)
{
	const int kRows = kMap.size(), kColumns = kMap[0].size();
	return (kPos.first >= 0 && kPos.first < kRows)
		&& (kPos.second >= 0 && kPos.second < kColumns);
}

int main(void)
{
	Map map;
	for (std::string line; std::getline(std::cin, line);)
	{
		map.emplace_back(line.begin(), line.end());
	}

	for (const auto &line : map)
	{
		for (const auto pos : line)
			std::cout << pos;
		std::cout << '\n';
	}

	const int kRows = map.size(), kColumns = map[0].size();
	Position pos;
	Direction dir = Direction::kUp;
	for (std::size_t i = 0; i < kRows; ++i)
	{
		for (std::size_t j = 0; j < kColumns; ++j)
			if (map[i][j] == '^')
				pos = std::make_pair(i, j);
	}

	std::cout << pos << '\n';
	while (InBounds(pos, map))
	{
		map[pos.first][pos.second] = 'X';

		using enum Direction;
		switch (dir)
		{
		case kUp:
		{
			const char kNeighbor = GetAdjacent(pos, kUp, map);
			if (IsObstacle(kNeighbor))
				dir = ChangeDirection(dir);
			else
				pos.first--;
			break;
		}
		case kDown:
		{
			const char kNeighbor = GetAdjacent(pos, kDown, map);
			if (IsObstacle(kNeighbor))
				dir = ChangeDirection(dir);
			else
				pos.first++;
			break;
		}
		case kLeft:
		{
			const char kNeighbor = GetAdjacent(pos, kLeft, map);
			if (IsObstacle(kNeighbor))
				dir = ChangeDirection(dir);
			else
				pos.second--;
			break;
		}
		case kRight:
		{
			const char kNeighbor = GetAdjacent(pos, kRight, map);
			if (IsObstacle(kNeighbor))
				dir = ChangeDirection(dir);
			else
				pos.second++;
			break;
		}
		}

		std::cout << pos << '\n';
	}

	const int kDistinctPositions = std::accumulate(
		map.cbegin(), map.cend(), 0,
		[](int acc, const auto &line) { return acc + std::ranges::count(line, 'X'); }
	);
	std::cout << kDistinctPositions << '\n';
	return 0;
}
