#include <cmath>
#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>


const long double EPS = 1e-9;


struct position_t {
    long double x;
    long double y;
    long double z;
};


using velocity_t = position_t;


struct hailstone_t {
    position_t p;
    velocity_t v;

public:
    std::optional<position_t> is_forward_intersection(const hailstone_t& h) const {
        long double denom = v.x * h.v.y - v.y * h.v.x;
        if (std::abs(denom) < EPS) {
            return std::nullopt;
        }

        long double t1 = ((h.p.x - p.x) * h.v.y - (h.p.y - p.y) * h.v.x) / denom;

        if (t1 < 0) {
            return std::nullopt;
        }

        long double t2 = ((p.x - h.p.x) * v.y - (p.y - h.p.y) * v.x) / (h.v.x * v.y - h.v.y * v.x);
        if (t2 < 0) {
            return std::nullopt;
        }

        long double x = p.x + t1 * v.x;
        long double y = p.y + t1 * v.y;

        return std::optional<position_t>({ x, y, 0.0 });
    }
};


std::ostream& operator<<(std::ostream& stream, const hailstone_t& h) {
    stream << h.p.x << ", " << h.p.y << ", " << h.p.z << " @ " << h.v.x << ", " << h.v.y << ", " << h.v.z;
    return stream;
}


std::ostream& operator<<(std::ostream& stream, const position_t& h) {
    stream << "x=" << h.x << ", y=" << h.y;
    return stream;
}


std::vector<hailstone_t> read_input() {
    std::fstream file("input.txt");
    std::vector<hailstone_t> result;

    for (std::string line; std::getline(file, line);) {
        std::stringstream ss(line);

        hailstone_t h;
        char ignore;

        ss >> h.p.x >> ignore >> h.p.y >> ignore >> h.p.z >> ignore >> h.v.x >> ignore >> h.v.y >> ignore >> h.v.z;
        result.push_back(h);
    }

    return result;
}


class solution {
private:
    std::vector<hailstone_t> m_hails;
    long double m_x_begin;
    long double m_x_end;
    long double m_y_begin;
    long double m_y_end;

public:
    solution(const std::vector<hailstone_t>& p_hails, long double x_begin, long double x_end, long double y_begin, long double y_end) :
        m_hails(p_hails),
        m_x_begin(x_begin),
        m_x_end(x_end),
        m_y_begin(y_begin),
        m_y_end(y_end)
    { }

public:
    int count_intersections() const {
        int counter = 0;
        for (int i = 0; i < m_hails.size(); i++) {
            for (int j = i + 1; j < m_hails.size(); j++) {
                const hailstone_t& a = m_hails[i];
                const hailstone_t& b = m_hails[j];

                auto intersection = a.is_forward_intersection(b);
                if (!intersection) {
                    continue;
                }

                const position_t p = intersection.value();
                if (p.x < m_x_begin || p.x > m_x_end || p.y < m_y_begin || p.y > m_y_end) {
                    continue;
                }

                counter++;
            }
        }

        return counter;
    }
};


int main() {
    auto input = read_input();

#if 0
    long double x1 = 7;
    long double x2 = 27;
    long double y1 = 7;
    long double y2 = 27;
#else
    long double x1 = 200000000000000;
    long double x2 = 400000000000000;
    long double y1 = 200000000000000;
    long double y2 = 400000000000000;
#endif
    int counter = solution(input, x1, x2, y1, y2).count_intersections();
    std::cout << "Intersections within the test area: " << counter << std::endl;

    return 0;
}
