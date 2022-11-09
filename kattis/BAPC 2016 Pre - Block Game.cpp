// https://open.kattis.com/problems/blockgame2

#include <cstdint>
#include <iostream>


class solution {
public:
    bool play(const std::uint64_t n, const std::uint64_t m) {
        bool me = true;

        std::uint64_t big = std::max(n, m);
        std::uint64_t small = std::min(n, m);

        while (big != 0 && small != 0) {
            std::uint64_t r = big % small;

            if (r == 0) {
                return me;
            }
            else {
                if (big / small >= 2) {
                    return me;
                }
                else {
                    std::uint64_t next_small = big - small;
                    big = small;
                    small = next_small;
                }
            }

            me = !me;
        }

        return me;
    }
};


int main() {
    std::uint64_t n, m;
    std::cin >> n >> m;

    std::cout << (solution().play(n, m) ? "win" : "lose") << std::endl;

    return 0;
}