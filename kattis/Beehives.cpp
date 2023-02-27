// 2014 Virginia Tech High School Programming Contest
// Beehives

#include <cmath>
#include <vector>

struct position {
    double x;
    double y;
};

class beehives_analyser {
private:
    

public:
    std::pair<int, int> analyse(const std::vector<position> & p_pos, const double distance) {
        int sour = 0;

        for (std::size_t i = 0; i < p_pos.size(); i++) {
            bool good_location = true;

            for (std::size_t j = 0; j < p_pos.size(); j++) {
                if (i == j) { continue; }

                double d = std::sqrt(std::pow(p_pos[i].x - p_pos[j].x, 2) + std::pow(p_pos[i].y - p_pos[j].y, 2));
                if (d <= distance) {
                    sour++;
                    good_location = false;
                    break;
                }
            }
        }

        return { sour, p_pos.size() - sour };
    }
};


#include <iostream>

int main() {
    while (true) {
        double d;
        int n;

        std::cin >> d >> n;
        if (d == 0.0 && n == 0) {
            return 0;
        }

        std::vector<position> hives;
        for (int i = 0; i < n; i++) {
            double x, y;
            std::cin >> x >> y;

            hives.push_back({ x, y });
        }

        auto result = beehives_analyser().analyse(hives, d);
        std::cout << result.first << " sour, " << result.second << " sweet" << std::endl;
    }

    return 0;
}