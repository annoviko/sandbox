#include <vector>
#include <numeric>


std::pair<int, int> solve_equation(int a, int b) {
    /* ax + by = 1 */
    if (std::gcd(a, b) != 1) {
        throw std::exception("The equation has more than one solution.");
    }

    bool is_swapped = false;
    if (a < b) {
        is_swapped = true;
        std::swap(a, b);
    }

    std::vector<int> c_cur = { 0, 1 };
    std::vector<int> c_prev = { 1, 0 };

    int r1 = a, r2 = b;
    while (r2 != 0) {
        int q = r1 / r2;
        int r = r1 % r2;

        if (r == 0) {
            break;
        }

        r1 = r2;
        r2 = r;

        /* { a[i], b[i] } = { a[i - 2], b[i - 2] } - q * { a[i - 1], b[i - 1]  } */
        std::vector<int> c = { c_prev[0] - q * c_cur[0], c_prev[1] - q * c_cur[1] };
        c_prev = c_cur;
        c_cur = c;
    }

    if (is_swapped) {
        std::swap(a, b);
        std::swap(c_cur[0], c_cur[1]);
    }

    /* verify result */
    if (a * c_cur[0] + b * c_cur[1] != 1) {
        throw std::exception("Incorrect answer obtained after solving linear equation.");
    }

    return { c_cur[0], c_cur[1] };
}


std::pair<int, int> crt(int a, int b, int c, int d) {
    /*
        Solve system of two congruences:

        x congruent a (mod b)
        x congruent c (mod d)

        gcd(b, d) = 1, otherwise exception
    */

    if (std::gcd(b, d) != 1) {
        throw std::exception("Incorrect input, CRT cannot be applied for modulo which is differ from '1'.");
    }

    const std::vector<int> ri = { a, c };
    const std::vector<int> Mi = { d, b };

    auto xy1 = solve_equation(Mi[0], b);  /* M1 * x + b * y = 1 */
    auto xy2 = solve_equation(Mi[1], d);

    const std::vector<int> xi = { xy1.first, xy2.first };

    int x = ri[0] * Mi[0] * xi[0] + ri[1] * Mi[1] * xi[1];
    int M = b * d;

    x = x % M;
    if (x < 0) {
        x += M;
    }

    /* verify result */
    if (((x - a) % b != 0) || ((x - c) % d != 0)) {
        throw std::exception("Incorrect answer obtained during solving CRT.");
    }

    return { x, M };
}


#include <iostream>

int main() {
    auto solver = [](int a, int b, int c, int d) {
        auto r = crt(a, b, c, d);
        std::cout << "x = " << r.first << " (mod " << r.second << ")." << std::endl;
    };

    try {
        solver(1, 3, 4, 5);
        solver(15, 32, 7, 19);
        solver(-1, 5, 3, 7);
        solver(-10, 13, -4, 17);
        solver(1234, 1009, 5678, 1013);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}