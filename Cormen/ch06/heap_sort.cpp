#include <algorithm>
#include <iostream>
#include <vector>


void max_heapify(const std::size_t p_index, std::vector<std::size_t> & p_array) {
	std::size_t left_index = (p_index + 1) << 1 - 1;
	std::size_t right_index = (p_index + 1) << 1;

	std::size_t largest_index = p_index;
	if (left_index < p_array.size() && p_array.at(p_index) < p_array.at(left_index)) {
		largest_index = left_index;
	}

	if (right_index < p_array.size() && p_array.at(largest_index) < p_array.at(right_index)) {
		largest_index = right_index;
	}

	if (p_index != largest_index) {
		std::swap(p_array[p_index], p_array[largest_index]);
		max_heapify(largest_index, p_array);
	}
}


void print_array(const std::vector<std::size_t> & p_array) {
	std::for_each(p_array.cbegin(), p_array.cend(), [](auto & p_elem) { std::cout << p_elem << " "; });
	std::cout << std::endl;
}


int main(int argc, char *argv[]) {
	std::vector<std::size_t> array = { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
	
	print_array(array);
	max_heapify(1, array);
	print_array(array);

	return 0;
}
