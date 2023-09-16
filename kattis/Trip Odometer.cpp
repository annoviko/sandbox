#include <algorithm>
#include <iostream>
#include <vector>
#include <set>


int main() {
    std::int64_t n = 0;
    std::cin >> n;

    std::vector<std::int64_t> content;
    std::int64_t total_sum = 0;
    for (std::int64_t i = 0; i < n; i++) {
        std::int64_t value;
        std::cin >> value;

        total_sum += value;
        content.push_back(value);
    }

    std::set<std::int64_t> result;
    for (std::int64_t i = 0; i < n; i++) {
        result.insert(total_sum - content[i]);
    }

    std::cout << result.size() << std::endl;

    bool is_first = true;
    for (auto item : result) {
        if (!is_first) {
            std::cout << ' ';
        }
        std::cout << item;
        is_first = false;
    }
    std::cout << std::endl;

    return 0;
}