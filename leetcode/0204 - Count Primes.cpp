class Solution {
public:
    int countPrimes(int n) {
        if (n <= 2) { return 0; } /* prime numbers that  less than n */
        
        std::vector<bool> table(n - 2, true); /* [0] is 2, [1] is 3, etc. */
        
        for (int i = 2; i < (n / 2) + 1; i++) { /* O(n) */
            for (int j = i + i; j < n; j += i) { /* log(log(n)) */
                table[j - 2] = false;
            }
        }
        
        int result = 0;
        for (auto value : table) {
            if (value) {
                result++;
            }
        }
        
        return result;
    }
};