#include <iostream>
#include <string>


int main() {
    std::string pattern;
    std::string text;

    while (std::getline(std::cin, pattern)) {
        std::getline(std::cin, text);

        std::size_t pos = text.find(pattern);
        while (pos != std::string::npos) {
            std::cout << pos << ' ';
            pos = text.find(pattern, pos + 1);
        }
        std::cout << std::endl;
    }

    return 0;
}