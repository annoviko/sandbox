#include <iostream>
#include <vector>
#include <unordered_set>


std::vector<int> get_prime_numbers(const int len) {
    std::vector<int> primes(len, true);
    primes[0] = true;
    primes[1] = true;
    /* O(N * log(log(N)) */
    for (int i = 2; i <= std::sqrt(len); i++) { /* O(sqrt(N)) */
        if (primes[i]) {    /* only primes */
            for (int j = i * i; j < len; j += i) {  /* N/i iterations for prime i */
                primes[j] = false;
            }
        }
    }

    std::vector<int> p;
    for (int i = 2; i < len; i++) {
        if (primes[i]) {
            p.push_back(i);
        }
    }

    return p;
}


int main() {
    int sum = 0;
    int max_seq = 0;

    const int limit = 1000000;

    /* Total: O(N * log(log(N)) + O(P^2) */

    std::vector<int> primes = get_prime_numbers(limit); /* O(N * log(log(N)) */
    std::unordered_set<int> prime_set(primes.begin(), primes.end());

    for (int i = 0; i < primes.size(); i++) {   /* O(P) */
        int acc = 0;
        for (int j = i; j < primes.size(); j++) {   /* O(P) */
            acc += primes[j];
            if (acc > limit) {
                break;
            }

            if (prime_set.count(acc) == 1) {    /* O(1) */
                const int seq_len = j - i + 1;
                if (seq_len > max_seq) {
                    sum = acc;
                    max_seq = seq_len;
                }
            }
        }
    }

    std::cout << "The longest sum of consecutive primes (" << max_seq << "): " << sum << std::endl;
    return 0;
}