#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n, m;
    std::cin >> n >> m;

    int east_max = -1;
    for (int i = 0; i < n; i++) {
        int candidate = 0;
        std::cin >> candidate;

        east_max = std::max(candidate, east_max);
    }

    int north_max = -1;
    for (int i = 0; i < m; i++) {
        int candidate = 0;
        std::cin >> candidate;

        north_max = std::max(candidate, north_max);
    }

    if (north_max != east_max) {
        std::cout << "impossible" << std::endl;
    }
    else {
        std::cout << "possible" << std::endl;
    }

    return 0;
}