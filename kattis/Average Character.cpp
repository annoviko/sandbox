#include <iostream>
#include <string>


int main() {
    std::string str;
    std::getline(std::cin, str);

    std::uint64_t sum = 0;
    for (char val : str) {
        sum += val;
    }

    char average = static_cast<char>(sum / str.size());
    std::cout << average << std::endl;
}