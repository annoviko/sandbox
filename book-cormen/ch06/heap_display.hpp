#include <iostream>
#include <cmath>


template <typename TypeValue>
std::size_t amount_digits(const TypeValue p_val) {
	return std::floor(std::log10(p_val)) + 1;
}


template <typename TypeArray>
void heap_display(const TypeArray & p_array) {
	max_length = 0;
	for (auto & val : p_array) {
		std::size_t amount = amount_digits(val);
		if (amount > max_length)
			max_length = amount;
	}

	const std::size_t height = std::floor(std::log2(p_array.size()));
	const std::size_t max_amount_leafs = std::pow(2, height);

	const std::size_t gap = 3;
	const std::size_t line_size = max_amount_leafs * (max_length + gap);


	for (std::size_t level = 0; level < height; level++) {
		std::size_t amount_element = std::pow(2, level);
		/* TODO */
	}
}
