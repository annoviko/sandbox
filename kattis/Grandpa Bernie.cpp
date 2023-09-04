#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, std::vector<int>> trips;

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string place;
        int year = 0;

        std::cin >> place >> year;

        trips[place].push_back(year);
    }

    for (auto& pair : trips) {
        std::sort(pair.second.begin(), pair.second.end());
    }

    int q = 0;
    std::cin >> q;

    for (int i = 0; i < q; i++) {
        std::string place;
        int index = 0;

        std::cin >> place >> index;

        std::cout << trips[place][index - 1] << std::endl;
    }

    return 0;
}