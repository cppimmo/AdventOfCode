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
#include <variant>
#include <unordered_map>
#include <sstream>
#include <deque>

using Grid = std::vector<std::vector<int>>;

static std::ostream &operator<<(std::ostream &outStream, const std::vector<int> &vec);
static std::ostream &operator<<(std::ostream &outStream, const Grid &grid);
static bool IsEdge(const Grid &grid, std::size_t row, std::size_t col);
static bool IsVisible(const Grid &grid, std::size_t row, std::size_t col);
static Grid GatherGrid(std::ifstream &inputFileStream);
static int CountVisibleTrees(const Grid &grid);

static std::size_t s_numRows, s_numCols;

int main(void)
{
	try
	{
		std::ifstream input("input.txt");
		if (!input.is_open())
			throw std::runtime_error("Input file not found!");

		auto grid = GatherGrid(input); // Store grid
		// Store number of rows and columns
		s_numRows = grid.size();
		s_numCols = grid[0].size();

		std::cout << grid;

		std::cout << "Number of visible trees = " << CountVisibleTrees(grid) << '\n';
		
		std::cin.get();
		return 0;
	}
	catch (const std::exception &except)
	{
		std::cerr << "Something went wrong: " << except.what() << '\n';
	}
}

std::ostream &operator<<(std::ostream &outStream, const std::vector<int> &vec)
{
	for (auto elem : vec)
		outStream << elem << ' ';
	
	return outStream;
}

std::ostream &operator<<(std::ostream &outStream, const Grid &grid)
{
	for (auto row : grid)
	{
		for (auto col : row)
			std::cout << col << ' ';
		
		std::cout << '\n';
	}
	return outStream;
}

bool IsEdge(const Grid &grid, std::size_t row, std::size_t col)
{
	return (row == 0
			|| row == (s_numRows - 1)
			|| col == 0
			|| col == (s_numCols - 1));
}

bool IsVisible(const Grid &grid, std::size_t row, std::size_t col)
{
	// Easiest check done first
	if (IsEdge(grid, row, col))
		return true;

	// Store current row and column
	auto rowVec = grid[row];
	auto colVec = [&grid](std::size_t col) -> std::vector<int>
	{
		std::vector<int> vec;

		for (std::size_t row = 0; row < s_numRows; ++row)
				vec.push_back(grid[row][col]);

		return vec;
	}(col);

	std::cout << "Row vec: " << rowVec << '\n';
	std::cout << "Col vec: " << colVec << '\n';
	
	int currHeight = 0, maxHeight = 0;
	currHeight = grid[row][col];
	
	if (currHeight == 0)
		return false;
	
	// Left check
	for (std::size_t index = 0; index < col; ++index)
	{
		maxHeight = std::max(maxHeight, rowVec[index]);
		
		if (maxHeight >= currHeight)
			break;
	}
	
	if (maxHeight < currHeight)
		return true;
	
// Right check
	maxHeight = 0;
	
	for (std::size_t index = col + 1; index < s_numCols; ++index)
	{
		maxHeight = std::max(maxHeight, rowVec[index]);
		
		if (maxHeight >= currHeight)
			break;
	}

	if (maxHeight < currHeight)
		return true;
	
	// Upwards check
	maxHeight = 0;

	for (std::size_t index = 0; index < row; ++index)
	{
		maxHeight = std::max(maxHeight, colVec[index]);
		
		if (maxHeight >= currHeight)
			break;
	}

	if (maxHeight < currHeight)
		return true;
	
	// Downwards check
	maxHeight = 0;

	for (std::size_t index = row + 1; index < s_numRows; ++index)
	{
		maxHeight = std::max(maxHeight, colVec[index]);

		if (maxHeight >= currHeight)
			break;
	}

	if (maxHeight < currHeight)
		return true;
	
	return false;
}

Grid GatherGrid(std::ifstream &inputFileStream)
{
	Grid resultGrid;
	
	for (std::string line; std::getline(inputFileStream, line); )
	{
		std::vector<int> heights;

		std::istringstream iss(line);
		int height;

		for (char ch : line)
			// See https://stackoverflow.com/questions/439573/how-to-convert-a-single-char-into-an-int
			heights.push_back(ch - '0');
		
		resultGrid.push_back(heights); // Push a row of heights
	}
	
	return resultGrid;
}

int CountVisibleTrees(const Grid &grid)
{
	int result = 0;
	
	for (std::size_t row = 0; row < s_numRows; ++row)
	{
		for (std::size_t col = 0; col < s_numCols; ++col)
		{
			if (IsVisible(grid, row, col))
				result += 1;
		}
	}

	return result;
}
