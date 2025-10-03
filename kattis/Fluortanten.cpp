#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    std::int64_t n = 0;
    std::cin >> n;

    std::int64_t bjorn_index = 0;
    std::int64_t orig_happines = 0;
    std::vector<std::int64_t> values(n, 0);
    for (std::int64_t i = 0; i < n; i++) {
        std::cin >> values[i];
        if (values[i] == 0) {
            bjorn_index = i;
        }

        orig_happines += (i + 1) * values[i];
    }

    std::int64_t best_happines = orig_happines;
    std::int64_t prev_happines = orig_happines;

    /* go right - O(N) */
    for (std::int64_t i = bjorn_index + 1; i < n; i++) {
        const std::int64_t k = i + 1;
        std::int64_t candidate = prev_happines - k * values[i] + (k - 1) * values[i];

        prev_happines = candidate;
        best_happines = std::max(candidate, best_happines);
    }

    prev_happines = orig_happines;

    /* go left - O(N) */
    for (std::int64_t i = bjorn_index - 1; i >= 0; i--) {
        const std::int64_t k = i + 1;
        std::int64_t candidate = prev_happines - (k - 1) * values[i] + k * values[i];

        prev_happines = candidate;
        best_happines = std::max(candidate, best_happines);
    }

    std::cout << best_happines << std::endl;

    return 0;
}