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


class intersection_checker {
private:
    std::vector<hailstone_t> m_hails;
    long double m_x_begin;
    long double m_x_end;
    long double m_y_begin;
    long double m_y_end;

public:
    intersection_checker(const std::vector<hailstone_t>& p_hails, long double x_begin, long double x_end, long double y_begin, long double y_end) :
        m_hails(p_hails),
        m_x_begin(x_begin),
        m_x_end(x_end),
        m_y_begin(y_begin),
        m_y_end(y_end)
    { }

public:
    int count() const {
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


/*

Hailstone position in time:

    position[t] = position[0] + t * velocity

For every hailstone:

    P[t][i] = P[0][i] + t * V[i]

The same for rock position:

    Pr[t] = Pr[0] + t * Vr

Rock and object collision:

    Po[t] = Pr[t]
    Po + t * Vo = Pr + t * Vr,   t - is the same for both parts of equation

    Po + t * Vo - Pr - t * Vr = 0
    (Po - Pr) + t(Vo - Vr) = 0
    t(Vo - Vr) = -(Po - Pr)
    t(Vo - Vr) = (Pr - Po)
    t = (Pr - Po) / (Vo - Vr)

We have three coordinates:

    t = (Xr - Xo) / (Vx_o - Vy_r) = (Yr - Yo) / (Vy_o - Vy_r) = (Zr - Zo) / (Vz_o - Vz_r)

From this, we can eliminate t and get two equations:

    (Xr - Xo) / (Vx_o - Vy_r) = (Yr - Yo) / (Vy_o - Vy_r)
    (Yr - Yo) / (Vy_o - Vy_r) = (Zr - Zo) / (Vz_o - Vz_r)

Which can be rewritten:

    (Xr - Xo) * (Vy_o - Vy_r) - (Yr - Yo) * (Vx_o - Vy_r) = 0                  (1)
    (Yr - Yo) * (Vz_o - Vz_r) - (Zr - Zo) * (Vy_o - Vy_r) = 0                  (2)

Open brackets for (1):

    Xr * Vy_o - Xr * Vy_r - Xo * Vy_o + Xo * Vy_r    - Yr * Vx_o + Yr * Vy_r + Yo * Vx_o - Yo * Vy_r = 0     (3)
                ^^^^^^^^^                                          ^^^^^^^^^

We have two non-linear components for (3), which can be eliminated by substructing two hailstones:

    Hailstone 1: Xr * Vy_o[1] - Xr * Vy_r - Xo[1] * Vy_o[1] + Xo[1] * Vy_r    - Yr * Vx_o[1] + Yr * Vy_r + Yo[1] * Vx_o[1] - Yo[1] * Vy_r = 0
    Hailstone 2: Xr * Vy_o[2] - Xr * Vy_r - Xo[2] * Vy_o[2] + Xo[2] * Vy_r    - Yr * Vx_o[2] + Yr * Vy_r + Yo[2] * Vx_o[2] - Yo[2] * Vy_r = 0

Their delta: 

    Xr * (Vy_o[1] - Vy_o[2]) - (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) + (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2]) - Vy_r * (Yo[1] - Yo[2]) = 0
                               ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vy_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])    (4)

Repeat the same for (2):

    (Yr - Yo) * (Vz_o - Vz_r) - (Zr - Zo) * (Vy_o - Vy_r) = 0

    Yr * Vz_o - Yr * Vz_r - Yo * Vz_o + Yo * Vz_r    - Zr * Vy_o + Zr * Vy_r + Zo * Vy_o - Zo * Vy_r = 0
                ^^^^^^^^^                                          ^^^^^^^^^

    Hailstone 1: Yr * Vz_o[1] - Yr * Vz_r - Yo[1] * Vz_o[1] + Yo[1] * Vz_r    - Zr * Vy_o[1] + Zr * Vy_r + Zo[1] * Vy_o[1] - Zo[1] * Vy_r = 0
    Hailstone 2: Yr * Vz_o[2] - Yr * Vz_r - Yo[2] * Vz_o[2] + Yo[2] * Vz_r    - Zr * Vy_o[2] + Zr * Vy_r + Zo[2] * Vy_o[2] - Zo[2] * Vy_r = 0

    Yr * (Vz_o[1] - Vz_o[2]) - (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) + (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = 0
                               ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Yr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])    (5)

Now we have linear forms in (4) and (5) for every pair of hailstones:

    Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vy_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])
    Yr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])



--------- To be continued ----------

    Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vy_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])

    A = [(Vy_o[1] - Vy_o[2]), (Xo[1] - Xo[2]), -(Vx_o[1] - Vx_o[2]), -(Yo[1] - Yo[2])]
    X = [Xr, Vy_r, Yr, Vy_r]
    B = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])

The same can 

*/
class trajectory_solver {
public:
    std::vector<hailstone_t> m_hails;

public:
    trajectory_solver(const std::vector<hailstone_t>& p_hails) :
        m_hails(p_hails)
    { }

public:

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
    int counter = intersection_checker(input, x1, x2, y1, y2).count();
    std::cout << "Intersections within the test area: " << counter << std::endl;

    return 0;
}
