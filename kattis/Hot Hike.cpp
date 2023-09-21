#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>


int main() {
    int n;
    std::cin >> n;

    std::vector<int> t(n);
    for (int i = 0; i < n; i++) {
        std::cin >> t[i];
    }

    int min_max_t = INT_MAX;
    int d = 0;

    for (int i = 2; i < n; i++) {
        int max = std::max(t[i - 2], t[i]);
        if (max < min_max_t) {
            min_max_t = max;
            d = i - 2;
        }
    }

    std::cout << d + 1 << " " << min_max_t << std::endl;

    return 0;
}