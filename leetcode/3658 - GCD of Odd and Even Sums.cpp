#include <numeric>

class Solution {
public:
    int gcdOfOddEvenSums(int n) {
        int sum_odd = (n * (n * 2)) / 2;
        int sum_even = (n * (n * 2 + 2)) / 2;

        return std::gcd(sum_odd, sum_even);
    }
};