#include <algorithm>
#include <iostream>
#include <cmath>
#include <memory>


constexpr long double EPS = 0.000000001;


struct point {
    using ptr = std::shared_ptr<point>;

    long double x = 0;
    long double y = 0;

public:
    point() = default;

    point(const long double p_x, const long double p_y) {
        x = p_x;
        y = p_y;
    }

public:
    bool is_diff(const point& p_other) const {
        return (std::fabs(x - p_other.x) > EPS) || (std::fabs(y - p_other.y) > EPS);
    }
};


class line {
private:
    long double a = 0.0;
    long double b = 0.0;
    long double c = 0.0;

public:
    line(const point& p1, const point& p2) {
        if (std::fabs(p1.x - p2.x) < EPS) {
            a = 1.0;
            b = 0.0;
            c = -p1.x;
        }
        else {
            a = -(p1.y - p2.y) / (p1.x - p2.x);
            b = 1.0;
            c = -a * p1.x - p1.y;
        }
    }

public:
    bool are_parallel(const line& l2) const {
        return (std::fabs(a - l2.a) < EPS && std::fabs(b - l2.b) < EPS);
    }

    point::ptr get_intersection(const line& l2) const {
        if (are_parallel(l2)) {
            return nullptr;
        }

        auto p = std::make_shared<point>();
        p->x = (l2.b * c - b * l2.c) / (l2.a * b - a * l2.b);
        if (std::fabs(b) > EPS) {
            p->y = -(a * p->x + c);
        }
        else {
            p->y = -(l2.a * p->x + l2.c);
        }

        return p;
    }
};


class segment {
private:
    point p1;
    point p2;
    line l;

public:
    segment(const point& p_p1, const point& p_p2) :
        p1(p_p1), p2(p_p2),
        l(p_p1, p_p2)
    { }

public:
    const line& get_line() const { return l; }

    bool inside(const point& p) const {
        const auto xmax = std::max(p1.x, p2.x);
        const auto xmin = std::min(p1.x, p2.x);

        const auto ymax = std::max(p1.y, p2.y);
        const auto ymin = std::min(p1.y, p2.y);

        if ((xmin <= p.x && p.x <= xmax) && (ymin <= p.y && p.y <= ymax)) {
            return true;
        }

        return false;
    }

    point::ptr get_intersection(const segment& p_other) const {
        const auto p = l.get_intersection(p_other.get_line());
        if (p == nullptr) {
            return nullptr;
        }

        if (inside(*p) && p_other.inside(*p)) {
            return p;
        }

        return nullptr;
    }
};


#include <vector>


class solution {
public:
    std::size_t count_triangles(const std::vector<segment>& p_segments) {
        std::size_t result = 0;

        for (std::size_t i = 0; i < p_segments.size(); i++) {
            for (std::size_t j = i + 1; j < p_segments.size(); j++) {
                const point::ptr p1 = p_segments[i].get_intersection(p_segments[j]);
                if (p1 == nullptr) {
                    continue;
                }

                for (std::size_t k = j + 1; k < p_segments.size(); k++) {
                    const auto p2 = p_segments[i].get_intersection(p_segments[k]);
                    const auto p3 = p_segments[j].get_intersection(p_segments[k]);

                    if ((p2 == nullptr) || (p3 == nullptr)) {
                        continue;
                    }

                    if (p1->is_diff(*p2) && p1->is_diff(*p3) && p2->is_diff(*p3)) {
                        result++;
                    }
                }
            }
        }

        return result;
    }
};


int main() {
    std::size_t n = 0;

    while (std::cin >> n && n != 0) {
        std::vector<segment> segments;
        for (std::size_t i = 0; i < n; i++) {
            point p1, p2;
            std::cin >> p1.x >> p1.y >> p2.x >> p2.y;

            segments.push_back(segment{ p1, p2 });
        }

        std::cout << solution().count_triangles(segments) << std::endl;
    }

    return 0;
}