#include <algorithm>
#include <iostream>
#include <vector>


void max_heapify_recursion(std::size_t p_index, std::vector<std::size_t> & p_array) {
	const std::size_t left_index = ((p_index + 1) << 1) - 1;
	const std::size_t right_index = (p_index + 1) << 1;

	std::cout << left_index << " " << right_index << std::endl;

	std::size_t largest_index = p_index;
	if (left_index < p_array.size() && p_array.at(p_index) < p_array.at(left_index)) {
		largest_index = left_index;
	}

	if (right_index < p_array.size() && p_array.at(largest_index) < p_array.at(right_index)) {
		largest_index = right_index;
	}

	if (p_index != largest_index) {
		std::swap(p_array[p_index], p_array[largest_index]);
		max_heapify_recursion(largest_index, p_array);
	}
}


void max_heapify_loop(std::size_t p_index, std::vector<std::size_t> & p_array) {
	std::size_t initial_index = p_index;

	do
	{
		const std::size_t left_index = ((p_index + 1) << 1) - 1;
		const std::size_t right_index = (p_index + 1) << 1;

		initial_index = p_index;
		std::size_t largest_index = p_index;

		if (left_index < p_array.size() && p_array.at(p_index) < p_array.at(left_index))
			largest_index = left_index;

		if (right_index < p_array.size() && p_array.at(largest_index) < p_array.at(right_index))
			largest_index = right_index;

		if (p_index != largest_index) {
			std::swap(p_array[p_index], p_array[largest_index]);
			p_index = largest_index;
		}
	}
	while(p_index != initial_index);
}


void print_array(const std::vector<std::size_t> & p_array) {
	std::for_each(p_array.cbegin(), p_array.cend(), [](auto & p_elem) { std::cout << p_elem << " "; });
	std::cout << std::endl;
}


int main(int argc, char *argv[]) {
	std::vector<std::size_t> array1 = { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
	std::vector<std::size_t> array2 = { 2, 14, 10, 8, 7, 9, 3, 2, 4, 1 };

	print_array(array1);
	max_heapify_loop(1, array1);
	print_array(array1);

	print_array(array2);
	max_heapify_loop(0, array2);
	print_array(array2);

	return 0;
}
