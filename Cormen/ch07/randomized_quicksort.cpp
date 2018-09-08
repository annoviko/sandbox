#include <algorithm>
#include <iostream>
#include <vector>
#include <random>


std::random_device device;
std::mt19937 generator(device());


std::size_t partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
	std::size_t lborder = p_begin - 1;
	for (std::size_t rborder = p_begin; rborder < p_anchor; rborder++) {
		if (p_array[rborder] <= p_array[p_anchor]) {
			std::swap(p_array[rborder], p_array[++lborder]);
		}
	}

	std::swap(p_array[++lborder], p_array[p_anchor]);
	return lborder;
}


std::size_t randomized_partition(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
	std::uniform_int_distribution<std::size_t> distribution(p_begin, p_anchor);
	auto random_anchor = distribution(generator);
	std::swap(p_array[random_anchor], p_array[p_anchor]);
	return partition(p_array, p_begin, p_anchor);
}


void randomized_quicksort(std::vector<std::size_t> & p_array, const std::size_t p_begin, const std::size_t p_anchor) {
	if (p_begin < p_anchor && p_anchor != (std::size_t) -1) {
		const auto border = randomized_partition(p_array, 0, p_anchor);
		randomized_quicksort(p_array, p_begin, border - 1);
		randomized_quicksort(p_array, border + 1, p_anchor);
	}
}


std::vector<std::vector<std::size_t>> samples = {
	{ 1, 2, 3, 4, 9, 3, 2 },
	{ 0, 2, 4, 2, 4, 1, 4 },
	{ 8, 3, 4, 1, 3, 9, 0, 1 },
	{ 1, 1, 1, 1, 3, 3, 3, 3 },
	{ 9, 2, 3, 0, 4, 5, 6 }
};


void test_sort(const std::vector<std::size_t> & p_array) {
	auto expected = p_array;
	auto actual = p_array;

	randomized_quicksort(actual, 0, p_array.size() - 1);
	std::sort(expected.begin(), expected.end());

	if (expected != actual) {
		std::cout << "[FAIL]" << std::endl;
	}
	else {
		std::cout << "[PASS]" << std::endl;
	}
}


int main(int argc, char * argv[]) {
	for (const auto & ar : samples) {
		test_sort(ar);
	}

	return 0;
}
