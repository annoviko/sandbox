#include <iostream>
#include <cstdint>


int main() {
    std::uint64_t value = 0;
    
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        value <<= 1;

        char animal;
        std::cin >> animal;

        value += (animal == 'O');
    }

    std::cout << value << std::endl;
    return 0;
}