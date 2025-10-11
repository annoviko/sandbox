#include <cmath>
#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>


const long double EPS = 1e-12;


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

    t = (Xr - Xo) / (Vx_o - Vx_r) = (Yr - Yo) / (Vy_o - Vy_r) = (Zr - Zo) / (Vz_o - Vz_r)

From this, we can eliminate t and get two equations:

    (Xr - Xo) / (Vx_o - Vy_r) = (Yr - Yo) / (Vy_o - Vy_r)
    (Yr - Yo) / (Vy_o - Vy_r) = (Zr - Zo) / (Vz_o - Vz_r)
    (Xr - Xo) / (Vx_o - Vx_r) = (Zr - Zo) / (Vz_o - Vz_r)

Which can be rewritten:

    (Xr - Xo) * (Vy_o - Vy_r) - (Yr - Yo) * (Vx_o - Vx_r) = 0                  (1)
    (Yr - Yo) * (Vz_o - Vz_r) - (Zr - Zo) * (Vy_o - Vy_r) = 0                  (2)
    (Xr - Xo) * (Vz_o - Vz_r) - (Zr - Zo) * (Vx_o - Vx_r) = 0                  (3)

Open brackets:

    Xr * Vy_o - Xr * Vy_r - Xo * Vy_o + Xo * Vy_r    - Yr * Vx_o + Yr * Vx_r + Yo * Vx_o - Yo * Vx_r = 0     (4)
                ^^^^^^^^^                                          ^^^^^^^^^

    Yr * Vz_o - Yr * Vz_r - Yo * Vz_o + Yo * Vz_r    - Zr * Vy_o + Zr * Vy_r + Zo * Vy_o - Zo * Vy_r = 0     (5)
                ^^^^^^^^^                                          ^^^^^^^^^

    Xr * Vz_o - Xr * Vz_r - Xo * Vz_o + Xo * Vz_r    - Zr * Vx_o + Zr * Vx_r + Zo * Vx_o - Zo * Vx_r = 0     (6)
                ^^^^^^^^^                                          ^^^^^^^^^

We have non-linear component im (4-6), eliminate by substructing two hailstones:

    For (4):

        Hailstone 1: Xr * Vy_o[1] - Xr * Vy_r - Xo[1] * Vy_o[1] + Xo[1] * Vy_r    - Yr * Vx_o[1] + Yr * Vx_r + Yo[1] * Vx_o[1] - Yo[1] * Vx_r = 0
        Hailstone 2: Xr * Vy_o[2] - Xr * Vy_r - Xo[2] * Vy_o[2] + Xo[2] * Vy_r    - Yr * Vx_o[2] + Yr * Vx_r + Yo[2] * Vx_o[2] - Yo[2] * Vx_r = 0

        Xr * (Vy_o[1] - Vy_o[2]) - (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) + (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2]) - Vx_r * (Yo[1] - Yo[2]) = 0
                                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])    (7)

    For (5):

        Hailstone 1: Yr * Vz_o[1] - Yr * Vz_r - Yo[1] * Vz_o[1] + Yo[1] * Vz_r    - Zr * Vy_o[1] + Zr * Vy_r + Zo[1] * Vy_o[1] - Zo[1] * Vy_r = 0
        Hailstone 2: Yr * Vz_o[2] - Yr * Vz_r - Yo[2] * Vz_o[2] + Yo[2] * Vz_r    - Zr * Vy_o[2] + Zr * Vy_r + Zo[2] * Vy_o[2] - Zo[2] * Vy_r = 0

        Yr * (Vz_o[1] - Vz_o[2]) - (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) + (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = 0
                                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        Yr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])    (8)

    For (6):

        Hailstone 1: Xr * Vz_o[1] - Xr * Vz_r - Xo[1] * Vz_o[1] + Xo[1] * Vz_r    - Zr * Vx_o[1] + Zr * Vx_r + Zo[1] * Vx_o[1] - Zo[1] * Vx_r = 0
        Hailstone 2: Xr * Vz_o[2] - Xr * Vz_r - Xo[2] * Vz_o[2] + Xo[2] * Vz_r    - Zr * Vx_o[2] + Zr * Vx_r + Zo[2] * Vx_o[2] - Zo[2] * Vx_r = 0

        Xr * (Vz_o[1] - Vz_o[2]) - (Xo[1] * Vz_o[1] - Xo[2] * Vz_o[2]) + Vz_r * (Xo[1] - Xo[2]) - Zr * (Vx_o[1] - Vx_o[2]) + (Zo[1] * Vx_o[1] - Zo[2] * Vx_o[2]) - Vx_r * (Zo[1] - Zo[2]) = 0
                                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        Xr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Xo[1] - Xo[2]) - Zr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Zo[1] - Zo[2]) = (Xo[1] * Vz_o[1] - Xo[2] * Vz_o[2]) - (Zo[1] * Vx_o[1] - Zo[2] * Vx_o[2])    (9)

The substruction result:

        Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])    (7)
        Yr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])    (8)
        Xr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Xo[1] - Xo[2]) - Zr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Zo[1] - Zo[2]) = (Xo[1] * Vz_o[1] - Xo[2] * Vz_o[2]) - (Zo[1] * Vx_o[1] - Zo[2] * Vx_o[2])    (9)

We have only 6 unknown variables: [Xr, Yr, Zr, Vx_r, Vx_y, Vx_z], it means we need at least 6 equations => 3 hailstones.

        1) Xr * (Vy_o[1] - Vy_o[2]) + Vy_r * (Xo[1] - Xo[2]) - Yr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Yo[1] - Yo[2]) = (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])
        
        2) Yr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Yo[1] - Yo[2]) - Zr * (Vy_o[1] - Vy_o[2]) - Vy_r * (Zo[1] - Zo[2]) = (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])
        
        3) Xr * (Vz_o[1] - Vz_o[2]) + Vz_r * (Xo[1] - Xo[2]) - Zr * (Vx_o[1] - Vx_o[2]) - Vx_r * (Zo[1] - Zo[2]) = (Xo[1] * Vz_o[1] - Xo[2] * Vz_o[2]) - (Zo[1] * Vx_o[1] - Zo[2] * Vx_o[2])

        4) Xr * (Vy_o[1] - Vy_o[3]) + Vy_r * (Xo[1] - Xo[3]) - Yr * (Vx_o[1] - Vx_o[3]) - Vx_r * (Yo[1] - Yo[3]) = (Xo[1] * Vy_o[1] - Xo[3] * Vy_o[3]) - (Yo[1] * Vx_o[1] - Yo[3] * Vx_o[3])
        
        5) Yr * (Vz_o[1] - Vz_o[3]) + Vz_r * (Yo[1] - Yo[3]) - Zr * (Vy_o[1] - Vy_o[3]) - Vy_r * (Zo[1] - Zo[3]) = (Yo[1] * Vz_o[1] - Yo[3] * Vz_o[3]) - (Zo[1] * Vy_o[1] - Zo[3] * Vy_o[3])
        
        6) Xr * (Vz_o[1] - Vz_o[3]) + Vz_r * (Xo[1] - Xo[3]) - Zr * (Vx_o[1] - Vx_o[3]) - Vx_r * (Zo[1] - Zo[3]) = (Xo[1] * Vz_o[1] - Xo[3] * Vz_o[3]) - (Zo[1] * Vx_o[1] - Zo[3] * Vx_o[3])

Form Ax = B:

    X = [Xr, Yr, Zr, Vx_r, Vy_r, Vz_r]


          (Vy_o[1] - Vy_o[2]),      -(Vx_o[1] - Vx_o[2]),                0,               -(Yo[1] - Yo[2]),       (Xo[1] - Xo[2]),             0
                   0,                (Vz_o[1] - Vz_o[2]),      -(Vy_o[1] - Vy_o[2]),              0,             -(Zo[1] - Zo[2]),      (Yo[1] - Yo[2])
    A =   (Vz_o[1] - Vz_o[2]),                0,               -(Vx_o[1] - Vx_o[2]),      -(Zo[1] - Zo[2]),              0,             (Xo[1] - Xo[2])
          (Vy_o[1] - Vy_o[3]),      -(Vx_o[1] - Vx_o[3]),                0,               -(Yo[1] - Yo[3]),       (Xo[1] - Xo[3]),             0
                   0,                (Vz_o[1] - Vz_o[3]),      -(Vy_o[1] - Vy_o[3]),              0,             -(Zo[1] - Zo[3]),      (Yo[1] - Yo[3])
          (Vz_o[1] - Vz_o[3]),                0,               -(Vx_o[1] - Vx_o[3]),      -(Zo[1] - Zo[3]),              0,             (Xo[1] - Xo[3])


          (Xo[1] * Vy_o[1] - Xo[2] * Vy_o[2]) - (Yo[1] * Vx_o[1] - Yo[2] * Vx_o[2])
          (Yo[1] * Vz_o[1] - Yo[2] * Vz_o[2]) - (Zo[1] * Vy_o[1] - Zo[2] * Vy_o[2])
    B =   (Xo[1] * Vz_o[1] - Xo[2] * Vz_o[2]) - (Zo[1] * Vx_o[1] - Zo[2] * Vx_o[2])
          (Xo[1] * Vy_o[1] - Xo[3] * Vy_o[3]) - (Yo[1] * Vx_o[1] - Yo[3] * Vx_o[3])
          (Yo[1] * Vz_o[1] - Yo[3] * Vz_o[3]) - (Zo[1] * Vy_o[1] - Zo[3] * Vy_o[3])
          (Xo[1] * Vz_o[1] - Xo[3] * Vz_o[3]) - (Zo[1] * Vx_o[1] - Zo[3] * Vx_o[3])

New we need to solve the problem using this Ax = B using LU or Gaussian elimination.
Solved using Gaussian elimination - see my comments below.

*/


using matrix = std::vector<std::vector<long double>>;
using vector = std::vector<long double>;


namespace math {
    /* 
    
    Gaussian elimination - an algorithm for solving systems of linear equations
    Link: https://en.wikipedia.org/wiki/Gaussian_elimination 
    
    Initially tried plain LU decomposition without pivoting, but it produced a singular matrix 
	for this problem. Tested LU on simple linear systems, and it worked correctly, but when 
	applied to AoC 2023 Day 24 input, the matrix was near-singular. Switching to Gaussian 
	elimination resolved the issue and it is way more simpler from the implementation point of view.
    Link: https://en.wikipedia.org/wiki/LU_decomposition

    */
    std::optional<vector> gaussian_elimination(matrix A, vector b) {
        int n = A.size();

        for (int k = 0; k < n; ++k) {
            int pivot = k;
            long double max_value = std::abs(A[k][k]);
            for (int i = k + 1; i < n; ++i) {
                if (std::abs(A[i][k]) > max_value) {
                    max_value = std::abs(A[i][k]);
                    pivot = i;
                }
            }

            if (max_value < EPS) {
                return { }; /* matrix is singular */
            }

            std::swap(A[k], A[pivot]);
            std::swap(b[k], b[pivot]);

            /* reduction */
            for (int i = k + 1; i < n; ++i) {
                long double factor = A[i][k] / A[k][k];

                for (int j = k; j < n; ++j) {
                    A[i][j] -= factor * A[k][j];
                }

                b[i] -= factor * b[k];
            }
        }

        /* back substitution */
        vector x(n);
        for (int i = n - 1; i >= 0; --i) {
            long double sum = b[i];

            for (int j = i + 1; j < n; ++j) {
                sum -= A[i][j] * x[j];
            }

            x[i] = sum / A[i][i];
        }

        return x;
    }
}


class trajectory_solver {
public:
    std::vector<hailstone_t> m_hails;

public:
    trajectory_solver(const std::vector<hailstone_t>& p_hails) :
        m_hails(p_hails)
    { }

public:
    std::uint64_t eval_rock_position() {
        for (int i = 0; i < m_hails.size(); i++) {
            for (int j = i + 1; j < m_hails.size(); j++) {
                for (int k = j + 1; k < m_hails.size(); k++) {
                    matrix A = create_a(i, j, k);
                    vector b = create_b(i, j, k);

                    std::optional<std::vector<long double>> x = math::gaussian_elimination(A, b);
                    if (x) {
                        hailstone_t r;
                        r.p.x = x.value()[0];
                        r.p.y = x.value()[1];
                        r.p.z = x.value()[2];

                        return (std::uint64_t) std::round(r.p.x + r.p.y + r.p.z);
                    }
                }
            }
        }

        return -1;
    }

private:
    matrix create_a(int i, int j, int k) {
        const auto [Xo1, Yo1, Zo1] = m_hails[i].p;
        const auto [Xo2, Yo2, Zo2] = m_hails[j].p;
        const auto [Xo3, Yo3, Zo3] = m_hails[k].p;

        const auto [Vx_o1, Vy_o1, Vz_o1] = m_hails[i].v;
        const auto [Vx_o2, Vy_o2, Vz_o2] = m_hails[j].v;
        const auto [Vx_o3, Vy_o3, Vz_o3] = m_hails[k].v;

        return {
            { (Vy_o1 - Vy_o2),  -(Vx_o1 - Vx_o2),          0,         -(Yo1 - Yo2),   (Xo1 - Xo2),        0      },
            {        0,          (Vz_o1 - Vz_o2),  -(Vy_o1 - Vy_o2),        0,       -(Zo1 - Zo2),   (Yo1 - Yo2) },
            { (Vz_o1 - Vz_o2),          0,         -(Vx_o1 - Vx_o2),  -(Zo1 - Zo2),        0,        (Xo1 - Xo2) },
            { (Vy_o1 - Vy_o3),  -(Vx_o1 - Vx_o3),          0,         -(Yo1 - Yo3),    (Xo1 - Xo3),        0     },
            {        0,          (Vz_o1 - Vz_o3),  -(Vy_o1 - Vy_o3),        0,        -(Zo1 - Zo3),  (Yo1 - Yo3) },
            { (Vz_o1 - Vz_o3),          0,         -(Vx_o1 - Vx_o3),  -(Zo1 - Zo3),        0,        (Xo1 - Xo3) }
        };

    }

    vector create_b(int i, int j, int k) {
        const auto [Xo1, Yo1, Zo1] = m_hails[i].p;
        const auto [Xo2, Yo2, Zo2] = m_hails[j].p;
        const auto [Xo3, Yo3, Zo3] = m_hails[k].p;

        const auto [Vx_o1, Vy_o1, Vz_o1] = m_hails[i].v;
        const auto [Vx_o2, Vy_o2, Vz_o2] = m_hails[j].v;
        const auto [Vx_o3, Vy_o3, Vz_o3] = m_hails[k].v;

        return {
            (Xo1 * Vy_o1 - Xo2 * Vy_o2) - (Yo1 * Vx_o1 - Yo2 * Vx_o2),
            (Yo1 * Vz_o1 - Yo2 * Vz_o2) - (Zo1 * Vy_o1 - Zo2 * Vy_o2),
            (Xo1 * Vz_o1 - Xo2 * Vz_o2) - (Zo1 * Vx_o1 - Zo2 * Vx_o2),
            (Xo1 * Vy_o1 - Xo3 * Vy_o3) - (Yo1 * Vx_o1 - Yo3 * Vx_o3),
            (Yo1 * Vz_o1 - Yo3 * Vz_o3) - (Zo1 * Vy_o1 - Zo3 * Vy_o3),
            (Xo1 * Vz_o1 - Xo3 * Vz_o3) - (Zo1 * Vx_o1 - Zo3 * Vx_o3)
        };
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
    int counter = intersection_checker(input, x1, x2, y1, y2).count();
    std::cout << "Intersections within the test area: " << counter << std::endl;

    std::uint64_t position = trajectory_solver(input).eval_rock_position();
    std::cout << "Sum of the X, Y, and Z coordinates of the rock initial position: " << position << std::endl;

    return 0;
}
