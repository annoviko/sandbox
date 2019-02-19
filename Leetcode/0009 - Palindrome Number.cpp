
class Solution {
public:
    bool isPalindrome(int x) {
#if defined(STR_SOLUTION)
        std::string str = std::to_string(x);
        for (std::size_t i = 0, j = str.size() - 1; i < j; i++, j--) {
            if (str[i] != str[j]) {
                return false;
            }
        }
        
        return true;
#else
        if (x < 0 || (x % 10 == 0 && x != 0))
            return false;
        
        int buffer = x;
        int revert = 0;
        while (buffer > 0) {
            revert = revert * 10 + buffer % 10;
            buffer /= 10;
        }
        
        return revert == x;
#endif
    }
};