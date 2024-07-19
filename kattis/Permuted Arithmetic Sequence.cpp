#include <iostream>
#include <vector>
#include <algorithm>


int main() {
    std::uint64_t t = 0;
    std::cin >> t;

    for (std::uint64_t i = 0; i < t; i++) {
        std::uint64_t n = 0;
        std::cin >> n;

        bool is_sorted = true;
        int is_ascended = -1;
        std::vector<int> seq;
        seq.reserve(n + 1);
        for (std::uint64_t j = 0; j < n; j++) {
            int value = 0;
            std::cin >> value;

            if (seq.empty()) { }
            else if (seq.size() == 1) {
                if (value <= seq.back()) {
                    is_ascended = 0;
                }
                else {
                    is_ascended = 1;
                }
            }
            else {
                if (value < seq.back() && is_ascended) {
                    is_sorted = false;
                }
                else if (value > seq.back() && !is_ascended) {
                    is_sorted = false;
                }
            }

            seq.push_back(value);
        }

        if (!is_sorted) {
            std::sort(seq.begin(), seq.end());
        }

        int delta = seq[1] - seq[0];
        bool has_resolution = false;
        for (int j = 2; j < n; j++) {
            int next_delta = seq[j] - seq[j - 1];
            if (next_delta != delta) {
                std::cout << "non-arithmetic" << std::endl;
                has_resolution = true;
                break;
            }
        }

        if (!has_resolution) {
            std::cout << (is_sorted ? "arithmetic" : "permuted arithmetic") << std::endl;
        }
    }

    return 0;
}
