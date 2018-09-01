#include <algorithm>
#include <iostream>
#include <vector>


void print_array(const std::vector<std::size_t> & p_array) {
        std::for_each(p_array.cbegin(), p_array.cend(), [](auto & p_elem) { std::cout << p_elem << " "; });
        std::cout << std::endl;
}


void max_heapify(const std::size_t p_index, std::vector<std::size_t> & p_array, const std::size_t p_length) {
	std::size_t left_index = ((p_index + 1) << 1) - 1;
	std::size_t right_index = (p_index + 1) << 1;

	std::size_t largest_index = p_index;
	if (left_index < p_length && p_array.at(p_index) < p_array.at(left_index)) {
		largest_index = left_index;
	}

	if (right_index < p_length && p_array.at(largest_index) < p_array.at(right_index)) {
		largest_index = right_index;
	}

	if (p_index != largest_index) {
		std::swap(p_array[p_index], p_array[largest_index]);
		max_heapify(largest_index, p_array, p_length);
	}
}


void build_max_heap(std::vector<std::size_t> & p_array) {
	const std::size_t first_leaf_index = (p_array.size() >> 1);
	for (std::size_t i = first_leaf_index; i > 0; i--) {
		max_heapify(i - 1, p_array, p_array.size());
	}
}


void heapsort(std::vector<std::size_t> & p_array) {
	build_max_heap(p_array);

	std::size_t heap_length = p_array.size();
	for (std::size_t i = 0; i < p_array.size(); i++) {
		std::swap(p_array[0], p_array[heap_length - 1]);
		heap_length--;
		max_heapify(0, p_array, heap_length);
	}
}


int main(int argc, char *argv[]) {
	std::vector<std::size_t> array1 = { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
	std::vector<std::size_t> array2 = { 2, 4, 6, 22, 12, 0, 3, 5, 2, 10 };
	std::vector<std::size_t> array3 = { 9, 2, 5, 6, 10, 12, 3, 2, 1, 10 };

	heapsort(array1);
	heapsort(array2);
	heapsort(array3);

	print_array(array1);
	print_array(array2);
	print_array(array3);

	return 0;
}
