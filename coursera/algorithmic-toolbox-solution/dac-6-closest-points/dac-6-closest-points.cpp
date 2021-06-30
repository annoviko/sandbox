#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::pair;
using std::min;


struct point {
public:
    int x;
    int y;
};


double euclidean_distance(const point & p1, const point & p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


double minimal_distance_recursion(const vector<point> & p_seq) {
    if (p_seq.size() <= 1) {
        return std::numeric_limits<double>::max();
        //return std::sqrt(std::pow(p_seq[0].x - p_seq[1].x, 2) + std::pow(p_seq[0].y - p_seq[1].y, 2));
    }

    std::size_t middle = p_seq.size() / 2;

    const double min_dist1 = minimal_distance_recursion(std::vector<point>(p_seq.begin(), p_seq.begin() + middle));
    const double min_dist2 = minimal_distance_recursion(std::vector<point>(p_seq.begin() + middle, p_seq.end()));

    const double min_current = std::min(min_dist1, min_dist2);

    // check on border points that are less than min_current
    const double x_border = (p_seq[middle - 1].x + p_seq[middle].x) / 2.0;

    // get points around the X-axis border from left and right
    std::vector<point> region;
    for (const auto & p : p_seq) {
        if (std::abs(x_border - p.x) < min_current) {
            region.push_back(p);
        }
    }

    std::sort(region.begin(), region.end(), [](point & p1, point & p2) {
        return p1.y < p2.y;
        });

    // find minimum within border
    double minimum = min_current;
    for (std::size_t i = 0; i < region.size(); i++) {
        for (std::size_t j = i + 1; j < region.size(); j++) {
            if (std::abs(region[i].y - region[j].y) > minimum) {
                break;
            }

            double distance = euclidean_distance(region[i], region[j]);
            minimum = std::min(distance, minimum);
        }
    }

    return minimum;
}


double minimal_distance(vector<point> p_seq) {
    std::sort(p_seq.begin(), p_seq.end(), [](point & p1, point & p2) {
        return p1.x < p2.x;
    });

    return minimal_distance_recursion(p_seq);
}


int main() {
    size_t n;
    std::cin >> n;

    std::vector<point> sequence(n);
    for (size_t i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        sequence[i] = { x, y };
    }
    std::cout << std::fixed;
    std::cout << std::setprecision(9) << minimal_distance(sequence) << "\n";
}
