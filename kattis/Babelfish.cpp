#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, std::string> dict;
    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        std::stringstream stream(line);

        std::string a, b;
        stream >> a >> b;

        dict[b] = a;
    }

    for (std::string word; std::getline(std::cin, word);) {
        auto iter = dict.find(word);
        if (iter != dict.cend()) {
            std::cout << iter->second << std::endl;
        }
        else {
            std::cout << "eh" << std::endl;
        }
    }

    return 0;
}