#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int area = 0;
    for (int h = 5; h <= 23; h++) {
        for (int w = 4; w <= 23; w++) {
            int p = (h + w) * 2 - 4;
            if (p <= n) {
                area++;
            }
        }
    }

    std::cout << area << std::endl;

    return 0;
}