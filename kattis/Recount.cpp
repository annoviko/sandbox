#include <iostream>
#include <string>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, int> votes;

    for (std::string line; std::getline(std::cin, line) && line != "***";) {
        votes[line]++;
    }

    int max_votes = 0;
    std::string name;

    for (const auto& pair : votes) {
        if (pair.second > max_votes) {
            max_votes = pair.second;
            name = pair.first;
        }
        else if (pair.second == max_votes) {
            name.clear();
        }
    }

    if (!name.empty()) {
        std::cout << name << std::endl;
    }
    else {
        std::cout << "Runoff!" << std::endl;
    }

    return 0;
}