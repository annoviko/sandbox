#include <iostream>
#include <vector>
#include <algorithm>

struct item {
    double  p_kilo_value;
    int     p_weight;
};


double get_optimal_value(int capacity, std::vector<int> weights, std::vector<int> values) {
    double value = 0.0;

    std::vector<item> items;
    for (std::size_t i = 0; i < weights.size(); i++) {
        items.push_back({ static_cast<double>(values[i]) / weights[i], weights[i] });
    }

    std::sort(items.begin(), items.end(), [](const item & p1, const item & p2) {
        return p1.p_kilo_value < p2.p_kilo_value;
    });

    for (auto it = items.crbegin(); (it != items.crend()) && (capacity > 0); it++) {
        const int remaining_weight = capacity - it->p_weight;
        if (remaining_weight > 0) {
            capacity = remaining_weight;
            value += it->p_kilo_value * it->p_weight;
        }
        else {
            value += it->p_kilo_value * capacity;
            capacity = 0;
        }
    }

    return value;
}


int main() {
    int n;
    int capacity;
    std::cin >> n >> capacity;
    std::vector<int> values(n);
    std::vector<int> weights(n);
    for (int i = 0; i < n; i++) {
        std::cin >> values[i] >> weights[i];
    }

    double optimal_value = get_optimal_value(capacity, weights, values);

    std::cout.precision(10);
    std::cout << optimal_value << std::endl;
    return 0;
}
