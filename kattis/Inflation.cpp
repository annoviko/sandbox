#include <iostream>
#include <algorithm>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> sources(n);
    for (int i = 0; i < n; i++) {
        std::cin >> sources[i];
    }

    std::sort(sources.begin(), sources.end());

    long double fraction = 1;
    bool possible = true;
    for (int i = 0; i < n; i++) {
        if (sources[i] <= (i + 1)) {
            const long double baloon_fraction = static_cast<long double>(sources[i]) / static_cast<long double>(i + 1);
            fraction = std::min(fraction, baloon_fraction);
        }
        else {
            possible = false;
            break;
        }
    }

    if (possible) {
        std::cout << fraction << std::endl;
    }
    else {
        std::cout << "impossible" << std::endl;
    }

    return 0;
}
