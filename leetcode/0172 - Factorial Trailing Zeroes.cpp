class Solution {
public:
    int trailingZeroes(int n) {
        int zeros = 0;
        while (n / 5 > 0) {
            int remain = n / 5;
            zeros += remain;
            n = remain;
        }
        
        return zeros;
    }
};