#include <iostream>
#include <vector>


int main() {
    const int len = 1000;

#if 0
    /* this code is to observe the pattern only when there is the difference */
    /* the pattern: [7] 1 [4] 1 [7] 1 [4] 1 ... */
    std::int64_t n = 3;
    std::int64_t d = 2;

    std::int64_t max_diff = 0;
    std::int64_t nm = 0, dm = 0;

    for (int i = 0; i < 45; i++) {
        std::int64_t delta = d;
        d = n + delta;
        n = d + delta;

        /* count difference */
        std::int64_t dc = std::log10(d);
        std::int64_t nc = std::log10(n);

        max_diff = std::max(nc - dc, max_diff);

        std::cout << "digits (" << dc << ", " << nc << ")" << n << '/' << d << " has difference in digits: " << nc - dc << std::endl;
    }
#endif

    /* counting fractions using pattern obrained before */
    /* the pattern: [7] 1 [4] 1 [7] 1 [4] 1 [7] 1 [4] 1 [7] ...*/
    int counter = 0;
    std::vector<int> pattern = { 7, 4 };
    for (int i = 8; i <= len;) {
        counter++;

        int index = counter % 2;
        i += pattern[index] + 1;
    }

    std::cout << "The number of fractions: " << counter << std::endl;
    return 0;
}