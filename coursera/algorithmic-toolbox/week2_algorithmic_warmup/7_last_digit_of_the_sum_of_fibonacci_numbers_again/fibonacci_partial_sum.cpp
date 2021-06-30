#include <iostream>
#include <vector>
using std::vector;

long long get_fibonacci_partial_sum_naive(long long from, long long to) {
    long long sum = 0;

    long long current = 0;
    long long next = 1;

    for (long long i = 0; i <= to; ++i) {
        if (i >= from) {
            sum += current;
        }

        long long new_current = next;
        next = next + current;
        current = new_current;

        //std::cout << current % 10 << " " << sum % 10 << std::endl;
    }

    return sum % 10;
}

long long get_fibonacci_partial_sum_fast(long long from, long long to) {
    if (to <= 1)
        return to;

    long long previous = 0;
    long long current = 1;

    long long bias = (from == 0) ? 1 : from;
    long long sum = 0;

    for (long long i = 0; i <= to; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = (tmp_previous + current) % 10;

        if (i >= from) {
            sum = (sum + tmp_previous) % 10;
        }

        //std::cout << previous << " " << sum << std::endl;
    }

    return sum;
}

void stress_test() {
    while (true) {
        long long from = rand() % 20;
        long long to = from + rand() % 20;

        if (get_fibonacci_partial_sum_naive(from, to) != get_fibonacci_partial_sum_fast(from, to)) {
            std::cout << "FAIL: " << from << ", " << to << std::endl;
        }
        else {
            std::cout << "OK" << std::endl;
        }
    }
}

int main() {
    long long from, to;
    std::cin >> from >> to;

    std::cout << get_fibonacci_partial_sum_fast(from, to) << std::endl;
}
