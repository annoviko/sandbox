#include <iostream>
#include <vector>
#include <algorithm>


void min_heapify(const std::size_t p_index, std::vector<std::size_t> & p_array) {
	std::size_t left_index = (p_index + 1) << 1 - 1;
	std::size_t right_index = (p_index + 1) << 1;

	std::size_t smallest_index = p_index;
	if (left_index < p_array.size() && p_array.at(left_index) < p_array.at(p_index)) {
		smallest_index = left_index;
	}

	if (right_index < p_array.size() && p_array.at(right_index) < p_array.at(smallest_index)) {
		smallest_index = right_index;
	}

	if (p_index != smallest_index) {
		std::swap(p_array[p_index], p_array[smallest_index]);
		min_heapify(smallest_index, p_array);
	}
}


void print_array(std::vector<std::size_t> & p_array) {
	for(auto & p_elem : p_array) {
		std::cout << p_elem << " ";
	}
	std::cout << std::endl;
}


int main(int argc, char * argv[]) {
	std::vector<std::size_t> array = { 0, 1, 20, 3, 4, 5, 6, 7, 8 };
	
	print_array(array);
	min_heapify(0, array);
	min_heapify(5, array);
	min_heapify(2, array);
	print_array(array);

	return 0;
}
