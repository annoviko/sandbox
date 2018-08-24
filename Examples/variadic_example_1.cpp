#include <iostream>
#include <string>
#include <sstream>
#include <vector>


template <typename TypeValue>
std::string to_string(const TypeValue & p_val) {
	std::stringstream stream;
	stream << p_val;
	return stream.str();
}


template <typename ... ParamType>
void println1(const ParamType& ... param) {
	std::vector<std::string> buffer = { to_string(param)... };
	for (const auto & value : buffer) {
		std::cout << value << " ";
	}
	std::cout << std::endl;
}


void println2() {
	std::cout << std::endl;
}


template <typename FirstValueType, typename ... ParamType>
void println2(const FirstValueType & p_first_value, const ParamType & ... p_param) {
	std::cout << p_first_value << " ";
	println2(p_param...);
}



int main(int argc, char *argv[]) {
	println1(1, 2, "Hello");
	println2(4, 'd', "World", 5.6);
	return 0;
}
