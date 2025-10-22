#include <iostream>
#include <vector>


double eval_c(int a, int b) {
    return std::sqrt(std::pow(a, 2) + std::pow(b, 2));
}


double func(int a, int b) {
    return a + b + eval_c(a, b);
}


int main() {
    const int target = 1000;
    bool found = false;

    for (int b = 1; b < (target / 3) && !found; b++) {    /* O(N/3 * log(N/2)) ~ O(N*log(N)) */
        int a_begin = 1;
        int a_end = (target / 2);

        while (a_begin < a_end) {   /* O(log(N/2)) */
            int a = (a_begin + a_end) / 2;

            double r = func(a, b);
            if (r > target) {
                a_end = a;
            }
            else if (r < target) {
                a_begin = a + 1;
            }
            else {
                int c = eval_c(a, b);
                std::cout << "a = '" << a << "', b = '" << b << "', c = '" << c << "'" << std::endl;
                std::cout << a * b * c << std::endl;
                found = true;
                break;
            }
        }
    }

    return 0;
}