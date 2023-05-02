#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>


struct point {
    long double x = 0;
    long double y = 0;
};


long double euclidean_distance(const point& p1, const point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


class solution {
public:
    long double get_smaller_distance(const point& g, const point& p1, const point& p2) {
        long double distance = std::numeric_limits<long double>::max();
        
        if (p1.x <= g.x && g.x <= p2.x) {
            distance = std::min(std::fabs(g.y - p1.y), std::fabs(g.y - p2.y));
        }
        else if (p1.y <= g.y && g.y <= p2.y) {
            distance = std::min(std::fabs(g.x - p1.x), std::fabs(g.x - p2.x));
        }
        else {
            std::vector<point> corners = { p1, { p1.x, p2.y }, p2, { p2.x, p1.y } };
            for (const auto& p : corners) {
                distance = std::min(distance, euclidean_distance(p, g));
            }
        }

        return distance;
    }
};


#include <iostream>
#include <iomanip>

int main() {
    point g, p1, p2;
    std::cin >> g.x >> g.y >> p1.x >> p1.y >> p2.x >> p2.y;

    std::cout << std::setprecision(10) << solution().get_smaller_distance(g, p1, p2) << std::endl;
    return 0;
}