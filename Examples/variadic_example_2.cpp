#include <iostream>
#include <vector>
#include <algorithm>


bool contain_negative1() {
	return false;
}


template <typename TypeFirstParam, typename ... TypeParam>
bool contain_negative1(const TypeFirstParam & p_first, const TypeParam & ... p_param) {
	return !(p_first >= 0) || contain_negative1(p_param ...);
}


template <typename TypeValue>
bool contain_negative2(const TypeValue & p_value) {
	return p_value < 0;
}


template <typename ... TypeParam>
bool contain_negative2(const TypeParam & ... p_param) {
	std::vector<bool> results = { contain_negative2(p_param)... };
	
	for (const auto & val : results) {
		if (val) { return true; }
	}
	
	return false;	
}


template <typename ... TypeParam>
bool contain_negative3(const TypeParam & ... p_param) {
	auto checker = [](const auto & p_val) { return p_val < 0; };
	std::vector<bool> results = { checker(p_param) ... };

	auto iter = std::find(results.begin(), results.end(), true);
	return iter != results.end();
}


int main(int argc, char *argv[]) {
	std::cout << "FIRST" << std::endl;
	std::cout << (contain_negative1(1, -2, 1, 4) ? "true" : "false") << std::endl;
	std::cout << (contain_negative1(1, 4, 2.0, 2) ? "true" : "false") << std::endl;
	std::cout << (contain_negative1(-1.0, -2.0) ? "true" : "false") << std::endl;
	std::cout << (contain_negative1(0, 0, 0, 0) ? "true" : "false") << std::endl;
	std::cout << std::endl;

	std::cout << "SECOND" << std::endl;
        std::cout << (contain_negative2(1, -2, 1, 4) ? "true" : "false") << std::endl;
        std::cout << (contain_negative2(1, 4, 2.0, 2) ? "true" : "false") << std::endl;
        std::cout << (contain_negative2(-1.0, -2.0) ? "true" : "false") << std::endl;
	std::cout << (contain_negative2(0, 0, 0, 0) ? "true" : "false") << std::endl;
        std::cout << std::endl;

        std::cout << "THIRD" << std::endl;
        std::cout << (contain_negative3(1, -2, 1, 4) ? "true" : "false") << std::endl;
        std::cout << (contain_negative3(1, 4, 2.0, 2) ? "true" : "false") << std::endl;
        std::cout << (contain_negative3(-1.0, -2.0) ? "true" : "false") << std::endl;
        std::cout << (contain_negative3(0, 0, 0, 0) ? "true" : "false") << std::endl;

	return 0;
}
