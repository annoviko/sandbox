#include <iostream>
#include <cstdlib>
#include <cmath>


#if 0
    #define TEST_ALGORITHM
#endif


std::int64_t count_0_aligned(std::int64_t to, std::int64_t order, std::int64_t len) {
    std::int64_t result = 1;
    for (std::int64_t i = len; i > 0; i--) {
        std::int64_t order_value = ((to - order) / 10) * i + i;   /* counts in zero aligned ranges (10, 90), (100, 900), (1000, 5000) */
        result += order_value;

        to = order - 1;
        if (to == 0) {
            to = order - 1;
        }

        order /= 10;
    }

    return result;
}


std::int64_t count_zeros_before_remaining(std::int64_t target, std::int64_t remaining) {
    std::int64_t target_length = std::log10(target);
    std::int64_t zeros_before = target_length - 1;

    std::int64_t to_add = 0;
    std::int64_t result = 0;

    std::int64_t cur_length = 1;
    std::int64_t remaining_order = 10;

    std::int64_t total_value = 0;
    while (zeros_before > 0) {
        if (remaining / remaining_order >= 1) {
            std::int64_t values = std::int64_t(std::pow(10, cur_length)) - std::int64_t(std::pow(10, cur_length - 1));
            total_value += values;
            result += values * zeros_before;
        }
        else {
            result += (remaining - total_value) * zeros_before;
            break;
        }

        remaining_order *= 10;
        zeros_before--;
        cur_length++;
    }

    return result;
}


std::int64_t count_0(std::int64_t to) {
    if (to == 0) {
        return 1;
    }
    else if (to < 0) {
        return 0;
    }

    std::int64_t order = 1;
    std::int64_t len = 0;
    while (10 * order <= to) {
        order *= 10;
        len++;
    }

    std::int64_t remain = to % order;
    std::int64_t value = to - remain;

    std::int64_t result = count_0_aligned(value, order, len);
    if (remain != 0) {
        result += count_zeros_before_remaining(value, remain);
        result += count_0(to - value) - 1 /* zero is already counted */;
    }

    return result;
}


std::int64_t count_0_in_range(std::int64_t from, std::int64_t to) {
    return count_0(to) - count_0(from - 1);
}


#if defined(TEST_ALGORITHM)
std::int64_t count_0_brute_force(std::int64_t from, std::int64_t to) {
    std::int64_t result = 0;

    for (std::int64_t i = from; i <= to; i++) {
        std::int64_t value = i;
        if (value == 0) {
            result++;
            continue;
        }

        while (value > 0) {
            if (value % 10 == 0) {
                result++;
            }

            value /= 10;
        }
    }

    return result;
}

void test_algorithm() {
    for (int i = 0; i < 10000; i++) {
        auto actual = count_0(i);
        auto expected = count_0_brute_force(0, i);

        if (actual != expected) {
            std::cout << "FAIL: '" << i << "' ('" << actual << "' != '" << expected << "')." << std::endl;
            std::exit(-1);
        }
    }

    std::cout << "PASS" << std::endl;

    for (int from = 1000; from < 10000; from++) {
        for (int to = from + 1; to < 10000; to++) {
            auto actual = count_0_in_range(from, to);
            auto expected = count_0_brute_force(from, to);

            if (actual != expected) {
                std::cout << "FAIL: range ('" << from << "', '" << to << "') ('" << actual << "' != '" << expected << "')." << std::endl;
                std::exit(-1);
            }
        }
    }

    std::cout << "PASS" << std::endl;
}
#endif


int main() {
#if defined(TEST_ALGORITHM)
    test_algorithm();
#else
    std::int64_t from, to;
    std::cin >> from >> to;

    while (from != -1 && to != -1) {
        std::cout << count_0_in_range(from, to) << std::endl;
        std::cin >> from >> to;
    }
#endif

    return 0;
}
