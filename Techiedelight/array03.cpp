/*

Given an binary array, sort it in linear time and constant space. Output should print contain all zeroes followed by all ones.


For example,

Input: { 1, 0, 1, 0, 1, 0, 0, 1 }
Output: { 0, 0, 0, 0, 1, 1, 1, 1 }

*/

#include <algorithm>
#include <vector>
#include <iostream>


void SortBinaryArray(std::vector<int> & pArray) {
	int count = std::accumulate(pArray.begin(), pArray.end(), 0);
	for (int i = 0; i < pArray.size() - count; i++) { pArray[i] = 0; }
	for (int i = count; i < pArray.size(); i++) { pArray[i] = 1; }
}


void PrintCollection(const std::vector<int> & pArray) {
	std::cout << "[ ";
	std::for_each(pArray.begin(), pArray.end(), [](auto & pElement) { std::cout << pElement << " "; });
	std::cout << "]" << std::endl;
}


int main(int argc, char *argv[]) {
	std::vector<int> sequence { 1, 0, 1, 0, 1, 0, 0, 1 };
	PrintCollection(sequence);
	SortBinaryArray(sequence);
	PrintCollection(sequence);
	return 0;
}
