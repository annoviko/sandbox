#include <iostream>
#include <string>


int main() {
    std::string line;
    std::cin >> line;

    std::string output;
    for (char c : line) {
        if (c == 'e') {
            output.push_back(c);
        }

        output.push_back(c);
    }

    std::cout << output << std::endl;
    return 0;
}