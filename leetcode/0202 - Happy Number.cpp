#include <unordered_set>


class Solution {
public:
    bool isHappy(int n) {
        std::unordered_set<int> loop_detector = { n };
        int value = n;

        while(value != 1) {
            value = check(value);
            if (loop_detector.count(value)) {
                return false;
            }

            loop_detector.insert(value);
        }

        return true;
    }

private:
    int check(int n) {
        int result = 0;
        while (n > 0) {
            int digit = n % 10;
            result += digit * digit;
            n /= 10;
        }
        return result;
    }
};
