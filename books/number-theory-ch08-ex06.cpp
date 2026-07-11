#include <iostream>
#include <numeric>
#include <vector>


class DiophantineSolver {
public:
    std::pair<int, int> solve(int a, int b) {
        if (a == 0 && b == 0) {
            return { 0, 0 };
        }
        else if (a == 0) {
            return { 0, 1 };
        }
        else if (b == 0) {
            return { 1, 0 };
        }

        bool is_swap = false;
        if (a < b) {
            std::swap(a, b);
            is_swap = true;
        }

        std::vector<std::pair<int, int>> coef{ { 0 , 1 } };
        int r1 = a, r2 = b;

        while ((r2 != 0) && (r1 % r2 != 0)) {
            int q = r1 / r2;
            int rn = r1 % r2;

            if (coef.size() == 1) {
                coef.push_back({ 1, -q });
            }
            else {
                const auto& c1 = coef[coef.size() - 2];
                const auto& c2 = coef[coef.size() - 1];

                const int a = c1.first - q * c2.first;
                const int b = c1.second - q * c2.second;

                coef.push_back({ a, b });
            }

            r1 = r2;
            r2 = rn;
        }

        if (a * coef.back().first + b * coef.back().second != r2) {
            throw std::runtime_error("incorrect solution");
        }

        int x = coef.back().first;
        int y = coef.back().second;

        if (is_swap) {
            std::swap(x, y);
        }

        return { x, y };
    }
};


class CongruentSolver {
public:
    std::vector<int> solve(int a, int c, int m) {
        const int g = std::gcd(a, m);
        if (c % g != 0) {
            return { }; /* no solutions */
        }

        std::vector<int> result;

        const auto uv = DiophantineSolver().solve(a, m);
        const int u = uv.first;
        const int v = -uv.second;

        for (int i = 0; i < g; i++) {
            int x = u * c / g + i * m / g;
            result.push_back(x);
        }

        return result;
    }
};


int main() {
    int a = 72, c = 47, m = 200;
    auto r = CongruentSolver().solve(a, c, m);
    if (r.empty()) {
        std::cout << "no solution" << std::endl;
    }
    else {
        for (int i = 0; i < r.size(); i++) {
            std::cout << "x congruent to " << r[i] << " (mod " << m << ")" << std::endl;
        }
    }
    return 0;
}