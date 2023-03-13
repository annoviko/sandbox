#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdint>


struct point {
    long double x = 0.0;
    long double y = 0.0;

    friend bool operator==(const point& l, const point& r) {
        return l.x == r.x && l.y == r.y;
    }
};


long double euclidean_distance(const point& p1, const point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


long double manhattan_distance(const point& p1, const point& p2) {
    return std::fabs(p1.x - p2.x) + std::fabs(p1.y - p2.y);
}


class triange {
private:
    std::vector<point> m_points;
    long double m_square = 0.0;

public:
    triange(const std::vector<point>& p_points) :
        m_points(p_points)
    {
        auto& p1 = m_points[0];
        auto& p2 = m_points[1];
        auto& p3 = m_points[2];

        m_square = std::fabs((p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p1.y * p2.x + p2.y * p3.x + p3.y * p1.x)) * static_cast<long double>(0.5);
    }

public:
    long double square() const { return m_square; }

    point left() const { return m_points[1]; }

    point right() const { return m_points[2]; }
};


class solution {
private:
    std::vector<point> m_figure;
    point m_door;

public:
    solution(const std::vector<point>& p_figure) :
        m_door(p_figure[0])
    {
        m_figure.push_back(p_figure[0]);
        for (std::int64_t i = p_figure.size() - 1; i > 0; i--) {
            m_figure.push_back(p_figure[i]);
        }
    }

public:
    point divide() {
        std::vector<triange> triangles;

        for (std::int64_t i = 2; i < m_figure.size(); i++) {
            triange t({ m_door, m_figure[i - 1], m_figure[i] });
            triangles.push_back(t);
        }

        std::vector<std::int64_t> bucket1;
        long double square1 = 0.0;

        std::vector<std::int64_t> bucket2;
        long double square2 = 0.0;

        std::int64_t begin = 0;
        std::int64_t end = triangles.size() - 1;
        while (begin <= end) {
            if (square1 < square2) {
                bucket1.push_back(begin);
                square1 += triangles[begin].square();
                begin++;
            }
            else {
                bucket2.push_back(end);
                square2 += triangles[end].square();
                end--;
            }
        }

        const long double total_area = square1 + square2;
        std::int64_t index_triangle = 0;

        long double square1_wo_argue = square1;
        long double square2_wo_argue = square2;
        if (square1 < square2) {
            index_triangle = bucket2.back();
            square2_wo_argue -= triangles[index_triangle].square();
        }
        else {
            index_triangle = bucket1.back();
            square1_wo_argue -= triangles[index_triangle].square();
        }

        const long double square1_before = square1_wo_argue;
        const long double square2_before = square2_wo_argue;

        const long double total_square = square1 + square2;

        /* binary search */
        point left = triangles[index_triangle].left();
        point right = triangles[index_triangle].right();
        point middle = left;

        const long double slope = (right.y - left.y) / (right.x - left.x);
        const long double correction_divider = slope * slope + static_cast<long double>(1.0);
        const long double y_intercept = left.y - slope * left.x;

        do {
            square1 = square1_wo_argue;
            square2 = square2_wo_argue;

            long double x = (left.x + right.x) / 2.0;
            long double y = (left.y + right.y) / 2.0;

            /* check if we are still on the segment of the triangle */
#if 1
            long double closest_x = (x + slope * y - slope * y_intercept) / correction_divider;
            long double closest_y = slope * closest_x + y_intercept;
            middle = { closest_x, closest_y }; /* correct coordinates */
#endif

            triange left_tr({ m_door, left, middle });
            triange full_tr({ m_door, left, right });

            square1 += left_tr.square();
            square2 += (full_tr.square() - left_tr.square());

            if (square1 > square2) {
                right = middle;
                square2_wo_argue = square2; /* we can assign this square to 2 */
            }
            else {
                left = middle;
                square1_wo_argue = square1; /* we can assign this square to 1 */
            }
        } while (manhattan_distance(left, right) > 0.000001);

        return middle;
    }
};


#include <iostream>
#include <iomanip>


void single_test(const std::vector<point>& p, const point& expected) {
    const double long tolerance = 0.0000001;
    point actual = solution(p).divide();
    if (std::abs(actual.x - expected.x) > tolerance || std::abs(actual.y - expected.y) > tolerance) {
        std::cout << "[FAIL]" << std::endl;
    }
    else {
        std::cout << "[PASS]" << std::endl;
    }
}


void test() {
    single_test({ { 0, 0 }, { 1, 1 }, { 1, 2 }, { 0, 3 }, { -1, 2 }, { -1, 1 } }, point{ 0, 3 });
    single_test({ { 0, 0 }, { 1000000, 1000000 }, { 1000000, 2000000 }, { 0, 3000000 }, { -1000000, 2000000 }, { -1000000, 1000000 } }, point{ 0, 3000000 });
    single_test({ { 0, -10000000 }, { 1000000, 1000000 }, { 1000000, 2000000 }, { 0, 3000000 }, { -1000000, 2000000 }, { -1000000, 1000000 } }, point{ 0, 3000000 });
    single_test({ { 0, 3000000 }, { -1000000, 2000000 }, { -1000000, 1000000 }, { 0, -10000000 }, { 1000000, 1000000 }, { 1000000, 2000000 } }, point{ 0, -10000000 });
    single_test({ { 0, 0 }, { 5, 0 }, { 5, 5 }, { 0, 5 } }, point{ 5, 5 });
    single_test({ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } }, point{ 1, 1 });
    single_test({ { 0, 0 }, { 1000000, 0 }, { 1000000, 1 }, { 0, 1 } }, point{ 1000000, 1 });
    single_test({ { -10000000, 0 }, { 10000000, 0 }, { 10000000, 1 }, { -10000000, 1 } }, point{ 10000000, 1 });
    single_test({ { -10000000, -10000000 }, { 10000000, -10000000 }, { 10000000, 10000000 }, { -10000000, 10000000 } }, point{ 10000000, 10000000 });

    single_test({ { 7, 1 }, { 8, 3 }, { 5, 5 }, { 2, 3 }, { 3, 1 } }, point{ 3.5, 4 });
    single_test({ { 2, 2 }, { 10, 3 }, { 6, 8 } }, point{ 8, 5.5 });
}


int main() {
#if 0
    test();
#else
    int n = 0;
    std::cin >> n;

    std::vector<point> p;
    for (int i = 0; i < n; i++) {
        long double x, y;
        std::cin >> x >> y;

        p.push_back({ x, y });
    }

    point r = solution(p).divide();
    std::cout << std::setprecision(30) << r.x << " " << r.y << std::endl;
#endif
    return 0;
}
