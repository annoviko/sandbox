#include <iostream>
#include <cstdint>


std::uint64_t arithmetic_sequence(std::uint64_t n, std::uint64_t a1, std::uint64_t a2) {
    return (n * (a1 + a2)) / 2;
}


std::uint64_t get_time_slow_down(std::uint64_t l, std::uint64_t r) {
    /* 
    Delta between levels in case of left: (2 - 1), (4 - 2), (7 - 3), ... = 1, 2, 3, 4, ...
      So using arithmetic sequence we can find T value after going left: 1 + Sn

    Delta between levels in case of right: (3 - 1), (6 - 3), (10 - 6), ... = 2, 3, 4, 5, ...
      So using arithmetic sequence we can find T value after going right: 1 + Sn

    But if we have to go to the left first, then we need to set appropriate a1 and a2:
      a1 = 2 + l
      a2 = a1 + (r - 1) * 1 - simply formula for Nth sequence member, where `1` is delta.

    */
    std::uint64_t a1 = 1;
    std::uint64_t a2 = a1 + (l - 1) * 1;
    std::uint64_t tl = 1 + arithmetic_sequence(l, a1, a2);

    a1 = 2 + l;
    a2 = a1 + (r - 1);
    std::uint64_t t = tl + arithmetic_sequence(r, a1, a2);

    return t;
}


int main() {
    std::uint64_t n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::uint64_t l, r;
        std::cin >> l >> r;

        std::cout << get_time_slow_down(l, r) << std::endl;
    }

    return 0;
}
