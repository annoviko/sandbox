#include <iostream>
#include <string>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cout << "Test " << i + 1 << std::endl;

        int rows = 0, cols = 0;
        std::cin >> rows >> cols;

        std::vector<std::string> output(rows);
        for (int j = rows - 1; j >= 0; j--) {
            std::string line(cols, ' ');
            for (int k = cols - 1; k >= 0; k--) {
                std::cin >> line[k];
            }

            output[j] = std::move(line);
        }

        for (const auto& line : output) {
            std::cout << line << std::endl;
        }
    }

    return 0;
}