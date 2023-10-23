#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::int64_t> values(n);
    for (int i = 0; i < n; i++) {
        std::int64_t value = 0;
        std::cin >> values[i];
    }

    std::vector<std::int64_t> l = { values[0] * values[0] };
    std::vector<std::int64_t> r = { values[n - 1] };

    for (int i = 1; i < n - 1; i++) {   /* O(N) */
        l.push_back(values[i] * values[i] + l.back());
    }

    for (int i = n - 2; i > 0; i--) {   /* O(N) */
        r.push_back(values[i] + r.back());
    }

    std::int64_t max = 0;
    for (int i = 0; i < l.size(); i++) {    /* O(N) */
        max = std::max(max, l[i] * r[l.size() - i - 1]);
    }

    std::cout << max << std::endl;

    /* Total algorithm complexity: O(N) */
    /* Total memory: O(N) */

    return 0;
}