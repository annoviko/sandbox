#include <cmath>


class Solution {
public:
    long long countCommas(long long n) {
        int length = 0;
        long long p = 10;
        while (p <= n) {
            p *= 10;
            length++;
        }

        if (length < 3) {
            return 0;
        }

        long long sum = 0;
        int commas_in_value = 0;
        int power = 3;

        while (power <= length) {
            long long values = std::pow(10, power) - std::pow(10, power - 3);

            sum += commas_in_value * values;
            commas_in_value++;
            power += 3;
        }

        long long values = (n - (std::pow(10, power - 3) - 1));
        sum += values * commas_in_value;

        return sum;
    }
};