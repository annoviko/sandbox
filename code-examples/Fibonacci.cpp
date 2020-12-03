#include <iostream>
#include <string>


int fibonacci1(const int number) {
	if (number == 0) { return 0; }
	else if (number == 1) { return 1; }
	
	int f0 = 0;
	int f1 = 1;
	
	for (int i = 1; i < number; i++) {
		int value = f0 + f1;
		f0 = f1;
		f1 = value;
	}
	
	return f1;
}


int fibonacci2_recursive(const int number, const int fx1, const int fx2) {
	return (number == 0) ? fx2 : fibonacci2_recursive(number - 1, fx2, fx1 + fx2); 
}
	

int fibonacci2(const int number) {	
	return (number == 0) ? 0 : fibonacci2_recursive(number - 1, 0, 1);
}


constexpr int fibonacci3(const int number) {
	return (number == 0) ? 0 : fibonacci2_recursive(number - 1, 0, 1);
}



int main(int argc, char * argv[]) {
	std::cout << "Fibonacci (N = '" << argv[1] << "'): '" << fibonacci2(std::stoi(argv[1])) << "'." << std::endl;
	std::cout << "Const Fibonacci (N = '10' [const value]): '" << fibonacci3(10) << "'." << std::endl;
	return 0;
}
