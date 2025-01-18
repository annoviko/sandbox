#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>


int main() {
    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<int> plots(n);
    std::vector<double> circle_houses(m + k);

    for (int i = 0; i < n; i++) {
        std::cin >> plots[i];
    }

    for (int i = 0; i < m; i++) {
        std::cin >> circle_houses[i];
    }

    const double multiplier = std::sqrt(2.0) / 2.0;
    for (int i = 0; i < k; i++) {
        int side = 0;
        std::cin >> side;

        double radius = multiplier * side;
        circle_houses[i + m] = radius;
    }

    std::sort(circle_houses.begin(), circle_houses.end());
    std::sort(plots.begin(), plots.end());

    int built_houses = 0;
    for (int i = 0, j = 0; i < circle_houses.size() && j < plots.size(); j++) {
        if (circle_houses[i] < plots[j]) {
            i++;
            built_houses++;
        }
    }

    std::cout << built_houses << std::endl;
    return 0;
}