#include <iostream>


template <typename TypeArg>
bool contains_negative(const TypeArg & p_val) {
	return p_val < 0;
}


template <typename TypeArg, typename ... TypeParam>
bool contains_negative(const TypeArg & p_val, const TypeParam & ... p_param) {
	return contains_negative(p_val) || contains_negative(p_param ...);
}


/* since c++ 17 */
template <typename ... TypeParam>
bool contains_negative2(const TypeParam & ... p_param) {
	return (... || (p_param < 0));
}


int main(int argc, char *argv[]) {
	bool result = contains_negative(0, 2, 3, 0, 10, 23, 104);
	std::cout << "1. Contains negative: '" << (result ? "true" : "false") << "'" << std::endl;	
	
	result = contains_negative(0, -1, 2, -3);
	std::cout << "2. Contains negative: '" << (result ? "true" : "false") << "'" << std::endl;

        result = contains_negative(-2, 1, 2, 3);
        std::cout << "3. Contains negative: '" << (result ? "true" : "false") << "'" << std::endl;

        result = contains_negative2(0, 2, 3, 0, 10, 23, 104);
        std::cout << "1*. Contains negative: '" << (result ? "true" : "false") << "'" << std::endl;

        result = contains_negative2(0, -1, 2, -3);
        std::cout << "2*. Contains negative: '" << (result ? "true" : "false") << "'" << std::endl;

	return 0;
}
