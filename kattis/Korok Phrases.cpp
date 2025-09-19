#include <string>
#include <unordered_set>
#include <iostream>


int main() {
    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    std::unordered_set<std::string> phrases;
    for (int i = 0; i < n; i++) {
        std::getline(std::cin, line);
        phrases.insert(line);
    }

    std::cout << phrases.size() << std::endl;

    return 0;
}