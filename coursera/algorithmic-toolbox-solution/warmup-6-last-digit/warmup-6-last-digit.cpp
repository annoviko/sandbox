#include <iostream>

int fibonacci_sum_naive(long long n) {
	if (n <= 1)
		return n;

	long long previous = 0;
	long long current = 1;
	long long sum = 1;

	for (long long i = 0; i < n - 1; ++i) {
		long long tmp_previous = previous;
		previous = current;
		current = tmp_previous + current;
		sum += current;
	}

	return sum % 10;
}

long long fibonacci_sum_fast(long long n) {
	if (n <= 1)
		return n;

	/* cycle size 60 - starts from 0 1 2 4 7 2
	
	0) 0
	1) 1
	2) 2
	3) 4
	...
	60) 0
	61) 1
	62) 2
	63) 4
	
	*/
	long long previous = 0;
	long long current = 1;

	long long new_n = n % 60;
	if (new_n == 0) {
		return 0;
	}

	for (long long i = 0; i < new_n - 1; ++i) {
		long long tmp_previous = previous;
		previous = current;
		current = (tmp_previous + current + 1) % 10;
	}

	return current;
}

void stress_test() {
	while (true) {
		long long n = std::rand() % 70 + 1;
		long long expected = fibonacci_sum_naive(n);
		long long actual = fibonacci_sum_fast(n);
		if (expected != actual) {
			std::cout << "FAIL: " << n << " " << expected << " != " << actual << std::endl;
			std::exit(-1);
		}
		else {
			std::cout << "OK" << std::endl;
		}
	}
}

int main() {
	//for (int i = 0; i < 70; i++) {
	//	std::cout << i << ") " << fibonacci_sum_naive(i) << std::endl;
	//}
	//return 0;
	//stress_test();
	long long n = 0;
	std::cin >> n;
	//std::cout << fibonacci_sum_naive(n) << std::endl;
	std::cout << fibonacci_sum_fast(n) << std::endl;
}
