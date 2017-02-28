/*

Given an array of integers, check if array contains a sub-array having 0 sum. Also, prints end-points of all such sub-arrays.

 


For example,

Input:  { 4, 2, -3, -1, 0, 4 }

Sub-arrays with 0 sum are

{ -3, -1, 0, 4 }
{ 0 }
 

Input:  { 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 }

Sub-arrays with 0 sum are

{ 3, 4, -7 }
{ 4, -7, 3 }
{ -7, 3, 1, 3 }
{ 3, 1, -4 }
{ 3, 1, 3, 1, -4, -2, -2 }
{ 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 }

*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>


void print(const auto & pCollection) {
	std::cout << "[";
	std::for_each(std::begin(pCollection), std::end(pCollection), [](auto & pElement) { std::cout << pElement << " "; });
	std::cout << "]" << std::endl;
}


/* O(n^2) */
void GetZeroArrays1(const std::vector<int> & pSequence, std::vector<std::vector<int>> & pArrays) {
	for (int i = 0; i < pSequence.size(); i++) {
		std::vector<int> candidate { };
		int sum = 0;
		
		for (int j = i; j < pSequence.size(); j++) {
			candidate.push_back(pSequence[j]);
			sum += pSequence[j];

			if (sum == 0) { pArrays.push_back(candidate); }
		}
	}
}



/* O(n), memory: O(n) */
void GetZeroArrays2(const std::vector<int> & pSequence, std::vector<std::vector<int>> & pArrays) {
	std::unordered_multimap<int, int> markers { { 0, -1} };
	int count = 0;
	
	for (int i = 0; i < pSequence.size(); i++) {
		count += pSequence[i];
		auto marker = markers.equal_range(count);
		
		if (marker.first != markers.end()) {
			markers.insert(std::make_pair(count, i));
		}
		else {
			for (auto & iter = marker.first; iter != marker.second; iter++) {
				std::vector<int> subarray { };
				for (int k = iter->first + 1; k <= i; k++) { 
					subarray.push_back(pSequence[k]); 
				}
				
				pArrays.push_back(subarray);
			}
		}
	}
}



int main(int argc, char * argv[]) {
	std::vector<int> sequence { 3, 4, -7, 3, 1, 3, 1, -4, -2, -2 };
	
	std::vector<std::vector<int>> arrays;
	
	GetZeroArrays1(sequence, arrays);
	std::for_each(std::begin(arrays), std::end(arrays), [](auto & pElement) { print(pElement); });
	
	GetZeroArrays2(sequence, arrays);
	std::for_each(std::begin(arrays), std::end(arrays), [](auto & pElement) { print(pElement); });
	
	return 0;	
}
