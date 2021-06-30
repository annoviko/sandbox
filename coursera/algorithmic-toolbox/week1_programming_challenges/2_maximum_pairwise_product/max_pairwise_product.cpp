#include <iostream>
#include <vector>
#include <algorithm>

int MaxPairwiseProduct(const std::vector<int>& numbers) {
    int max_product = 0;
    int n = numbers.size();

    for (int first = 0; first < n; ++first) {
        for (int second = first + 1; second < n; ++second) {
            max_product = std::max(max_product,
                numbers[first] * numbers[second]);
        }
    }

    return max_product;
}

long long MaxPairwiseProductFast(const std::vector<int>& numbers) {
    if (numbers.size() < 2) {
        return 0;
    }

    std::size_t index1 = -1, index2 = -1;

    for (std::size_t i = 0; i < numbers.size(); i++) {
        if ((index1 == -1) || (numbers[i] >= numbers[index1])) {
            index1 = i;
        }
    }

    for (std::size_t i = 0; i < numbers.size(); i++) {
        if ((i != index1) && ((index2 == -1) || (numbers[i] >= numbers[index2]))) {
            index2 = i;
        }
    }

    return static_cast<long long>(numbers[index1]) * static_cast<long long>(numbers[index2]);
}

int main() {
#if 0
    while (true) {
        std::size_t n = rand() % 10 + 2;
        std::vector<int> numbers(n);
        for (std::size_t i = 0; i < n; i++) {
            numbers[i] = rand() % 10;
        }

        if (MaxPairwiseProduct(numbers) != MaxPairwiseProductFast(numbers)) {
            std::cout << "WRONG" << std::endl;
            for (auto value : numbers) {
                std::cout << value << ' ';
            }

            return 0;
        }
        else {
            std::cout << "OK" << std::endl;
        }
    }
#endif

    int n;
    std::cin >> n;
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << MaxPairwiseProductFast(numbers) << "\n";
    return 0;
}
