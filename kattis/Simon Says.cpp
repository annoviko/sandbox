#include <iostream>
#include <string>
#include <sstream>


int main() {
    std::string temp;
    std::getline(std::cin, temp);

    int n = std::stoi(temp);

    for (int i = 0; i < n; i++) {
        std::getline(std::cin, temp);
        std::stringstream stream(temp);

        std::string first, second;
        stream >> first >> second;
        if (first == "simon" && second == "says") {
            bool is_first = true;
            while (stream >> temp) {
                if (!is_first) {
                    std::cout << ' ';
                }

                std::cout << temp;
                is_first = false;
            }
            std::cout << std::endl;
        }
        else {
            std::cout << std::endl;
        }
    }

    return 0;
}