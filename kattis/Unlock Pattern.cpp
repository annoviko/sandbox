#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>


struct position {
    double long x = 0;
    double long y = 0;
};


double long euclidean_distance(const position& p1, const position& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


class solution {
public:
    double long get_unlock_length(const std::vector<std::string>& p_pattern) {
        std::unordered_map<char, position> locations;
        for (int i = 0; i < p_pattern.size(); i++) {
            for (int j = 0; j < p_pattern.size(); j++) {
                locations[p_pattern[i][j]] = { (long double) j, (long double) i };
            }
        }

        double long distance = 0;
        for (int i = 2; i <= 9; i++) {
            const auto& p1 = locations['0' + i - 1];
            const auto& p2 = locations['0' + i];

            distance += euclidean_distance(p1, p2);
        }

        return distance;
    }
};


#include <iostream>
#include <iomanip>

int main() {
    std::vector<std::string> pattern;
    for (int i = 0; i < 3; i++) {
        std::string row;
        for (int j = 0; j < 3; j++) {
            char value = 0;
            std::cin >> value;
            row.push_back(value);
        }

        pattern.push_back(row);
    }

    std::cout << std::setprecision(15) << solution().get_unlock_length(pattern) << std::endl;
    return 0;
}