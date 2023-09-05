#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    std::map<std::string, std::set<std::string>> names;
    std::unordered_map<std::string, int> first_name_counter;

    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        std::stringstream stream(line);

        std::string first, second;
        stream >> first >> second;

        first_name_counter[first]++;
        names[second].insert(first);
    }

    for (const auto& pair : names) {
        for (const auto& first_name : pair.second) {
            if (first_name_counter[first_name] == 1) {
                std::cout << first_name << std::endl;
            }
            else {
                std::cout << first_name << " " << pair.first << std::endl;
            }
        }
    }

    return 0;
}