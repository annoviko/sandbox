#include <iostream>
#include <cstdint>


int main() {
    int n = 0;
    std::cin >> n;

    std::int64_t initial = 0;
    std::int64_t balance = 0;
    for (int i = 0; i < n; i++) {
        std::int64_t transaction = 0;
        std::cin >> transaction;

        balance += transaction;
        if (balance < 0) {
            initial += (-balance);
            balance = 0;
        }
    }

    std::cout << initial << std::endl;

    return 0;
}