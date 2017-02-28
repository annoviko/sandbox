/*

Given an unsorted array of integers, find a pair with given sum in it .

Input:
arr = [8, 7, 2, 5, 3, 1]
sum = 10

Output:
Pair found at index 0 and 2	(8 + 2)
OR
Pair found at index 1 and 5 (7 + 3)

*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>


/* O(n^2) */
std::pair<int, int> GetPairSum1(const std::vector<int> & pArray, const int pSum) {
	for (int i = 0; i < pArray.size() - 1; i++) {
		if (pArray[i] < pSum) {
			for (int j = i + 1; j < pArray.size(); j++) {
				if (pArray[i] + pArray[j] == pSum) {
					return std::make_pair(pArray[i], pArray[j]);
				}
			}
		}
	}
	
	return std::make_pair(-1, -1);
}


/* O(n log(n)) */
std::pair<int, int> GetPairSum2(const std::vector<int> & pArray, const int pSum) {
	std::vector<int> sortedArray = pArray;
	std::sort(sortedArray.begin(), sortedArray.end());
	
	std::size_t lowIndex = 0;
	std::size_t highIndex = sortedArray.size() - 1;
	
	while(lowIndex < highIndex) {
		if (sortedArray[lowIndex] + sortedArray[highIndex] == pSum) {
			return std::make_pair(sortedArray[lowIndex], sortedArray[highIndex]);
		}
		
		(sortedArray[lowIndex] + sortedArray[highIndex] < pSum) ? lowIndex++ : highIndex--; 
	}
	
	return std::make_pair(-1, -1);
}


int main(int argc, char * argv[]) {
	std::vector<int> sequence { 8, 7, 2, 5, 3, 1 };
	auto pair = GetPairSum2(sequence, 10);
	
	if (std::get<0>(pair) != -1) {
		auto value1 = std::get<0>(pair);
		auto value2 = std::get<1>(pair);
		std::cout << "Pair found (" << value1 << " + " << value2 << ")" << std::endl;
	}
	else {
		std::cout << "Pair is not found." << std::endl;
	}
	
	return 0;
}
