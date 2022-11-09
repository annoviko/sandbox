#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>


struct point {
public:
    int x;
    int y;

public:
    bool operator==(const point& p_other) {
        return x == p_other.x && y == p_other.y;
    }
};

struct edge {
    point p1;
    point p2;
    double dist;
};


class solution {
private:
    double euclidean(const point& l, const point& r) {
        return std::sqrt(std::pow(l.x - r.x, 2) + std::pow(l.y - r.y, 2));
    }

public:
    point find_square_point(const std::vector<point>& p_points) {
        std::vector<edge> lines;
        for (std::size_t i = 0; i < p_points.size(); i++) {
            for (std::size_t j = i + 1; j < p_points.size(); j++) {
                const double dist = euclidean(p_points[i], p_points[j]);
                lines.push_back({ p_points[i], p_points[j], dist });
            }
        }

        std::sort(lines.begin(), lines.end(), [](const edge& l, const edge& r) { return l.dist < r.dist; });

        edge a = lines[0];
        edge b = lines[1];

        point anchor, p1, p2, other;
        if (a.p1 == b.p1) {
            anchor = a.p1; p1 = a.p2; p2 = b.p2;
        }
        else if (a.p1 == b.p2) {
            anchor = a.p1; p1 = a.p2; p2 = b.p1;
        }
        else if (a.p2 == b.p1) {
            anchor = a.p2; p1 = a.p1; p2 = b.p2;
        }
        else { // a.p2 == b.p2
            anchor = a.p2; p1 = a.p1; p2 = b.p1;
        }

        point c = { p1.x + p2.x, p1.y + p2.y }; /* coordinates from (0, 0) */
        c = { c.x - anchor.x, c.y - anchor.y }; /* append offset */

        return c;
    } 
};


int main() {
    std::vector<point> points;
    for (std::size_t i = 0; i < 3; i++) {
        int x, y;
        std::cin >> x >> y;
        points.push_back({x, y});
    }

    const point other = solution().find_square_point(points);
    std::cout << other.x << " " << other.y << std::endl;

    return 0;
}