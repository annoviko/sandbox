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

        m_square = 0.5 * std::abs((p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p1.y * p2.x + p2.y * p3.x + p3.y * p1.x));
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

        /* inidicate that we cannot do anything */
        if ((square1 == 0.0) && (square2 == 0.0)) {
            throw std::exception();
        }

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

        const long double total_square = square1 + square2;

        /* binary search */
        point left = triangles[index_triangle].left();
        point right = triangles[index_triangle].right();
        point middle = left;

        do {
            square1 = square1_wo_argue;
            square2 = square2_wo_argue;

            long double x = (left.x + right.x) / 2.0;
            long double y = (left.y + right.y) / 2.0;
            middle = { x, y };

            triange left_tr({ m_door, left, middle });
            triange right_tr({ m_door, middle, right });

            square1 += left_tr.square();
            square2 += right_tr.square();

            if (square1 > square2) {
                right = middle;
                square2_wo_argue = square2; /* we can assign this square to 2 */
            }
            else {
                left = middle;
                square1_wo_argue = square1; /* we can assign this square to 1 */
            }
        } 
        while (std::abs(left.x - right.x) + std::abs(left.y - right.y) > 0.000001);

        return middle;
    }
};


#include <iostream>
#include <iomanip>


int main() {
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

    return 0;
}
