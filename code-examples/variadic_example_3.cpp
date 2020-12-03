#include <iostream>
#include <vector>
#include <algorithm>


template <typename ... TypeParam>
auto sum1(const TypeParam & ... p_param) {
	return (... + p_param);
}


template <typename TypeArg>
auto sum2(const TypeArg & p_first) {
	return p_first;
}


template <typename TypeArg, typename ... TypeParam>
auto sum2(const TypeArg & p_first, const TypeParam & ... p_param) {
	return p_first + sum2(p_param ...);
}



int main(int argc, char *argv[]) {
	auto result1 = sum1(1, 2, 3);
	std::cout << "Result: " << result1 << std::endl;
	
	auto result2 = sum2(-1, -2, -3);
	std::cout << "Result: " << result2 << std::endl;

	auto result3 = sum2(std::string("H"), std::string("e"), std::string("ll"), std::string("o"));
	std::cout << "Result: " << result3 << std::endl;

	return 0;
}
