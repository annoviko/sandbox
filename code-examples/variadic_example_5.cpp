#include <iostream>
#include <tuple>


template <typename TypeArg>
bool is_monotonic(const TypeArg & p_val) {
	return true;
}


template <typename TypeArg, typename ... TypeParam>
bool is_monotonic(const TypeArg & p_val1, const TypeArg & p_val2, const TypeParam & ... p_param) {
	return (p_val1 < p_val2 && is_monotonic(p_val2, p_param ...));
}


/* since c++17 */
template <typename ... TypeParam>
bool is_monotonic2(const TypeParam & ... p_param) {
	/* check types */
	std::vector<std::size_t> types;
	using element_type = typename std::tuple_element<0, std::tuple<TypeParam ...>>::type;
	for (std::size_t i = 1; i < amount_params; i++) {
		using element_type = typename std::tuple_element<i, std::tuple<TypeParam ...>>::type;
		types.push_back(typeid(element_type).hash_code());
	}

	using element_type = typename std::tuple_element<0, std::tuple<TypeParam ...>>::type;
}


int main(int argc, char *argv[]) {
	bool result = is_monotonic(1, 2, 3, 4, 5, 6);
	std::cout << "1. Monotonic: '" << (result ? "true" : "false") << "'" << std::endl;

	result = is_monotonic(1, 2, 5, 4, 5, 6);
        std::cout << "2. Monotonic: '" << (result ? "true" : "false") << "'" << std::endl;

	result = is_monotonic(3, 6, 8, 10, 100, 101, 102);
        std::cout << "3. Monotonic: '" << (result ? "true" : "false") << "'" << std::endl;

	return 0;
}
