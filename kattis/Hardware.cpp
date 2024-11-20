#include <iostream>
#include <string>
#include <sstream>
#include <vector>


int main() {
    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    for (int i = 0; i < n; i++) {
        std::string address;
        std::getline(std::cin, address);

        std::string houses_line;
        std::getline(std::cin, houses_line);

        std::stringstream stream(houses_line);

        int houses = 0;
        stream >> houses;

        std::vector<int> digits(10, 0);
        while (houses > 0) {
            std::getline(std::cin, line);
            if (line[0] != '+') {
                houses--;

                for (char digit : line) {
                    digits[digit - '0']++;
                }

                continue;
            }

            std::stringstream ss(line);
            
            char symbol;
            int begin, end, interval;

            ss >> symbol >> begin >> end >> interval;

            for (int i = begin; i <= end; i += interval) {
                int value = i;
                while (value != 0) {
                    int digit = value % 10;
                    value /= 10;

                    digits[digit]++;
                }

                houses--;
            }
        }

        std::cout << address << std::endl;
        std::cout << houses_line << std::endl;

        int total_digits = 0;
        for (int i = 0; i < digits.size(); i++) {
            std::cout << "Make " << digits[i] << " digit " << i << std::endl;
            total_digits += digits[i];
        }

        std::cout << "In total " << total_digits << ' ' << (total_digits != 1 ? "digits" : "digit") << std::endl;
    }

    return 0;
}