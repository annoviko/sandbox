/*
Find a duplicate element in a limited range array
Given a limited range array of size n where array contains elements between 1 to n-1 with one element repeating, find the duplicate number in it.


For example,

Input:  { 1, 2, 3, 4, 4 }
Output: The duplicate element is 4
 

Input:  { 1, 2, 3, 4, 2 }
Output: The duplicate element is 2
*/


#include <vector>
#include <list>
#include <string>

#include <iostream>
#include <iterator>

#include "array04.hpp"


void displayDuplicate2(std::vector<int> & pCollection) {
	printCollection(pCollection);
	
	for (int i = 0; i < pCollection.size(); i++) {
		int value = pCollection[i];
	
		if (pCollection[value - 1] > 0) {
			pCollection[value - 1] = -pCollection[value - 1];
		}
		else {
			std::cout << "Diplicate : " << value << std::endl << std::endl;
			break;
		}
	}
}


void displayDuplicate3(const std::vector<int> & pCollection) {
	printCollection(pCollection);
	
	int xor_value = 0;
	for (auto & element : pCollection) {
		std::cout << xor_value << " " << element << std::endl;
		xor_value ^= element;
	}
	
	for (int i = 1; i <= std::distance(std::begin(pCollection), std::end(pCollection)); i++) {
		std::cout << "#";
		xor_value ^= i;
	}
	
	std::cout << "Diplicate : " << xor_value << std::endl << std::endl;
}



int main(int argc, char *argv[]) {
	std::vector<int> vec1 = { 1, 2, 3, 4, 4 };
	std::vector<int> vec2 = { 1, 2, 3, 3, 4 };
	double ar1[] = { 1.0, 2.5, 3.5, 2.5, 8.0 };
	std::list<std::string> list1 = { "Hello", "World", "Hello" };
	
	std::cout << "Display Duplicate 1" << std::endl;
	displayDuplicate1(vec1);
	displayDuplicate1(vec2);
	displayDuplicate1(ar1);
	displayDuplicate1(list1);
	
	std::cout << "Display Duplicate 2" << std::endl;
	auto copy_vector2 = vec2;
	displayDuplicate2(copy_vector2);
	
	std::vector<int> vec3 = { 1, 2, 3, 4, 4 };
	std::cout << "Display Duplicate 3" << std::endl;
	displayDuplicate3(vec1);

	return 0;
}
