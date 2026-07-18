#include <iostream>
#include <cmath>
#include <vector>


std::vector<int> g_primes;


std::vector<int> generate_prime_numbers(int limit) {
    const int sqrt_limit = std::sqrt(limit);

    std::vector<bool> prime_table(limit, true);
    prime_table[0] = false;
    prime_table[1] = false;

    for (int i = 2; i < sqrt_limit; i++) {
        if (prime_table[i]) {
            for (int j = i * i; j < limit; j += i) {
                prime_table[j] = false;
            }
        }
    }

    std::vector<int> primes;
    for (int i = 2; i < prime_table.size() && i <= sqrt_limit; i++) {
        if (prime_table[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}


std::vector<std::pair<int, int>> factorization(int value) {
    std::vector<std::pair<int, int>> factors;
    int cur_value = value;
    for (const int p : g_primes) {
        int power = 0;
        while (cur_value % p == 0) {
            power++;
            cur_value /= p;
        }

        if (power > 0) {
            factors.push_back({ p, power });
        }
    }

    return factors;
}


int int_pow(int value, int power) {
    int result = 1;
    for (int i = 0; i < power; i++) {
        result *= value;
    }

    return result;
}


int euler_phi_function(int value) {
    if (value == 1) {
        return 1;
    }

    const std::vector<std::pair<int, int>> pf = factorization(value);
    int phi_value = int_pow(pf[0].first, pf[0].second) - int_pow(pf[0].first, pf[0].second - 1);

    for (int i = 1; i < pf.size(); i++) {
        phi_value *= int_pow(pf[i].first, pf[i].second) - int_pow(pf[i].first, pf[i].second - 1);
    }

    return phi_value;
}


int main() {
    std::vector<int> input = { 1, 2, 3, 4, 5, 10, 11, 1000000 };
    g_primes = generate_prime_numbers(*std::max_element(input.begin(), input.end()));

    for (auto v : input) {
        std::cout << "Euler Phi function for '" << v << "': " << euler_phi_function(v) << std::endl;
    }

    return 0;
}