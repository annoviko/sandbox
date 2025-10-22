#include <iostream>
#include <vector>


int main() {
    const int power = 1000;

    std::vector<int> value = { 2 };
    for (int i = 2; i <= power; i++) {
        int c = 0;
        for (int j = 0; j < value.size(); j++) {
            const int r = value[j] * 2 + c;
            c = r / 10;

            value[j] = r % 10;
        }

        if (c > 0) {
            value.push_back(c);
        }
    }

    int sum = 0;
    for (int digit : value) {
        sum += digit;
    }

    std::cout << sum << std::endl;
    return 0;
}