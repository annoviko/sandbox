#include <iostream>
#include <cstdint>

#if 1
#define MEMORIZATION
#endif


#if defined(MEMORIZATION)
#include <unordered_map>

std::unordered_map<std::uint64_t, bool> cache;   /* [cur_value] -> [result] */
#endif

bool is_stan_win(std::uint64_t cur_value, std::uint64_t target) {
    if (cur_value >= target) {
        return false;   /* somebody won on the previous step */
    }

    if (cur_value * 9 >= target) {
        return true;
    }


#if defined(MEMORIZATION)
    auto iter = cache.find(cur_value);
    if (iter != cache.cend()) {
        return iter->second;
    }
#endif

    bool result = false;
    for (std::uint64_t i = 2; i <= 9; i++) {
        if (!is_stan_win(cur_value * i, target)) {
            result = true;
            break;
        }
    }

#if defined(MEMORIZATION)
    cache[cur_value] = result;
#endif
    return result;
}


int main() {
    std::uint64_t target = 0;
    while (std::cin >> target) {
        cache.clear();
        std::cout << (is_stan_win(1, target) ? "Stan wins." : "Ollie wins.") << std::endl;
    }

    return 0;
}