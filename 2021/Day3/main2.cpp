#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <algorithm>
#include <ranges>
#include <tuple>
#include <functional>
#include <utility>

static constexpr std::size_t gBitsSize{ 5 }; // 5 for test input.
using Bitset = std::bitset<gBitsSize>;
using BitsetVector = std::vector<Bitset>;
using OccurenceVector = std::vector<int>;
// An array of BitsSize vectors.
using OccurenceArray = std::array<OccurenceVector, gBitsSize>;
using CountArray = std::array<std::tuple<int, int>, gBitsSize>;
// Functions.
static OccurenceArray CalculateOccurences(const BitsetVector &);
static CountArray CountOccurences(const OccurenceArray &);

int
main(int argc, char *argv[])
{
	BitsetVector bitsVector{};
	{ // Grabbing each bitset from the input file.
		std::ifstream inputFile;
		static constexpr const char *pFileName = "input_test.txt";
		inputFile.open(pFileName); // input_test.txt
		if (!inputFile.is_open())
			return 1;

		auto bitsetFromString = [](const std::string &string) -> auto {
			return Bitset(string);
		};
		
		std::string line{};
		while (inputFile.peek() != EOF && std::getline(inputFile, line)) {
			std::cout << line << '\n';
			bitsVector.push_back(bitsetFromString(line));
			// std::cout << bits.to_string()  << '\n';
		}
	}
	/*
	 * Calculate the life support rating.
	 * Equals the product of oxygen generator and C02 scrubber ratings.
	 * We resue the prior diagnostic report.
	 * Begin with the first bit of the numbers in the report.
	 * Consider the bit criteria alongside other instructions.
	 */
	/*
	 * Oxygen Generator Rating Bit Criteria:
	 *
	 */
    /* for (const auto &bits : bitsVector)
		std::cout << bits.to_string() << '\n'; */
	std::array<std::tuple<int, int>, gBitsSize> countArray{};
	for (std::size_t indexB{ gBitsSize }; indexB != 0; --indexB) {
		const auto indexB2{ indexB - 1 };
		int offBits{ 0 }, onBits{ 0 };
		std::cout << "\nBit Position: " << indexB + 1 << "\n\n";
		for (auto iter = bitsVector.begin(); iter != bitsVector.end(); ++iter) {
			std::cout << iter->operator[](indexB2) << '\n';
			if (iter->test(indexB2))
				onBits += 1;
			else
				offBits += 1;
		}
		countArray[indexB] = std::make_tuple(offBits, onBits);
	}

	/* for (std::size_t index{ gBitsSize }; index != 0; --index) {
		const auto index2{ index - 1 };
		const auto [offBits, onBits] = countArray[index2];
		std::cout << index  << "|OffBits: " << offBits << ", OnBits: "
				  << onBits << '\n';
	} */

	enum struct CommonBit { Off, On, Equal };
	auto commomBitToString = [](CommonBit commonBit) -> std::string_view {
		using namespace std::string_view_literals;
		switch (commonBit) {
		case CommonBit::Off:
			return { "Off"sv };
			break;
		case CommonBit::On:
			return { "On"sv };
			break;
		case CommonBit::Equal:
			return { "Equal"sv };
			break;
		}
		return { ""sv };
	};
	auto mostCommonBitAt = [&countArray = std::as_const(countArray)](
		const decltype(countArray) &counts,
		std::size_t bitNumber) -> CommonBit {
		const auto [offBits, onBits] = counts[bitNumber];
		std::greater<decltype(offBits)> comparator{};
		if (comparator(offBits, onBits))
			return CommonBit::Off;
		else if (comparator(onBits, offBits))
			return CommonBit::On;
		return CommonBit::Equal;
	};

	for (std::size_t index{ 0 }; index < gBitsSize; ++index) {
		const auto commonBit = mostCommonBitAt(countArray, index);
		std::cout << commomBitToString(commonBit) << '\n';
	}


	// OccurenceArray occurenceArray = CalculateOccurences(bitsVector);
	/*
	 * C02 Scrubber Rating Bit Criteria:
	 * 
	 */

	// Show final output.
	
	/* // Calculate the occurences of each bitset in column major.
	OccurenceArray occurenceArray = CalculateOccurences(bitsVector);
	std::cout << "\n\n";
	// Generate an array of tuples that displays holds the number of On/Off bits.
	const CountArray countArray = CountOccurences(occurenceArray);
	
	Bitset gammaRate{}, epsilonRate{};
	for (decltype(countArray)::size_type i = 0; i < countArray.size(); ++i) {
		const auto [offBits, onBits] = countArray[i];
		std::greater<decltype(offBits)> comparator{};
		if (comparator(offBits, onBits))
			gammaRate.reset(i);
		else
			gammaRate.set(i);

		if (comparator(onBits, offBits))
			epsilonRate.reset(i);
		else
			epsilonRate.set(i);
	}
	std::cout << "Power consumption(Gamma rate: " << gammaRate.to_ulong()
			  << "(0b" << gammaRate.to_string() << "),\n\tEpsilon rate: "
			  << epsilonRate.to_ulong() << "(0b" << epsilonRate.to_string()
			  << epsilonRate << "))\n";
	std::cout << "Total power consumption: "
			  << (gammaRate.to_ulong() * epsilonRate.to_ulong()) << '\n'; */
	return 0;
}

static OccurenceArray
CalculateOccurences(const BitsetVector &vector)
{
	OccurenceArray result{};
	std::ranges::for_each(result, [&vector](auto &element) -> void {
		element.resize(vector.size());
	});
	// Column major iteration.
	for (std::size_t indexB{ 0 }; indexB < gBitsSize; ++indexB) {
		std::cout << '\n' << "Bit position: " << indexB + 1 << "\n\n";
		for (BitsetVector::size_type indexV{ 0 }; indexV < vector.size();
			 ++indexV) {
			const bool testResult = vector[indexV].test(indexB);
			result[indexB][indexV] = (testResult) ? 1 : 0;
			std::cout << std::boolalpha << testResult << '\n';
		}
	}
	return result;
}
/*
 * Tuple element 0 holds off bits, while tuple element 1 holds on bits.
 */
static CountArray
CountOccurences(const OccurenceArray &array)
{
	using SizeType = OccurenceArray::size_type;
	using DifferenceType = std::iterator_traits<
		OccurenceArray::const_iterator>::difference_type;
    CountArray result{};
	for (SizeType index{ 0 }; index < array.size(); ++index) {
		DifferenceType offBits = std::ranges::count(array[index], 0);
		DifferenceType onBits = array[index].size() - offBits;
		result[index] = std::make_tuple(offBits, onBits);
		std::cout << "OffBits: " << offBits << ", OnBits: " << onBits << '\n';
	}
	return result;
}

