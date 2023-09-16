#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>


int main() {
    std::int64_t n;
    std::cin >> n;

    std::vector<std::int64_t> values(n);
    for (std::int64_t i = 0; i < n; i++) {
        std::cin >> values[i];
    }

    std::sort(values.begin(), values.end());
    
    std::int64_t sum = 0;
    for (std::int64_t i = 1; i < n; i++) {
        sum += std::pow(values[i - 1] - values[i], 2);
    }

    std::cout << sum << std::endl;

    return 0;
}