#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, int> freq;

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string costume;
        std::cin >> costume;

        freq[costume]++;
    }

    std::vector<std::string> optimal;
    int min = std::numeric_limits<int>::max();

    for (const auto& pair : freq) {
        if (pair.second < min) {
            optimal = { pair.first };
            min = pair.second;
        }
        else if (pair.second == min) {
            optimal.push_back(pair.first);
        }
    }

    std::sort(optimal.begin(), optimal.end());

    for (const auto costume : optimal) {
        std::cout << costume << std::endl;
    }

    return 0;
}