#include <iostream>
#include <vector>


std::vector<int> polynomial_congruence(const std::vector<int>& polynom, const int m) {
    std::vector<int> solutions;

    for (int x = 0; x < m; x++) {
        std::int64_t value = 0;

        /* Horner's method */
        /* F(x) = a0 + x(a1 + x(a2 + x(a3 + ... + x(an-1 + xan) ... )) */

        /* 1) an */
        /* 2) x * an + an-1 */
        /* 3) x * (x * an + an-1) + an-2 */
        /* ... ... */
        for (int c : polynom) {
            value = (value * x + c) % m;
        }

        if (value < 0) {
            value += m;
        }

        if (value == 0) {
            solutions.push_back(x);
        }
    }

    return solutions;
}


void present_solve_congruence(const std::vector<int>& polynom, const std::vector<int>& mvec) {
    for (int m : mvec) {
        std::cout << "Solution for x^11 + 21*x^7 - 8*x^3 + 8 (mod " << m << "):" << std::endl;
        const auto solutions = polynomial_congruence(polynom, m);

        if (solutions.empty()) {
            std::cout << "\tNo solution for the congruence." << std::endl;
        }
        else {
            for (int x : solutions) {
                std::cout << "\tx = " << x << ";" << std::endl;
            }
        }

        std::cout << std::endl;
    }
}


int main() {
    present_solve_congruence({ 1, 0, 0, 0, 21, 0, 0, 0, -8, 0, 0, 8 }, { 130, 137, 144, 151, 158, 165, 172 });
    present_solve_congruence({ 1, 5, 4, -6, -4 }, { 11 });

    return 0;
}
