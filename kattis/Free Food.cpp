#include <iostream>
#include <unordered_set>


int main() {
    int n;
    std::cin >> n;

    std::unordered_set<int> free_food;

    for (int i = 0; i < n; i++) {
        int from, to;
        std::cin >> from >> to;

        for (int j = from; j <= to; j++) {
            free_food.insert(j);
        }
    }

    std::cout << free_food.size() << std::endl;

    return 0;
}