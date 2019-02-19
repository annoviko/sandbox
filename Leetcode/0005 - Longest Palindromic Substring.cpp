class Solution {
public:
    std::string longestPalindrome(std::string s) {
        std::size_t best_begin = 0, best_end = 0;
        std::size_t best_size = 0;
        
        for (std::size_t begin = 0; begin < s.size(); begin++) {
            const std::size_t max_size = s.size() - begin;
            if (max_size < best_size) {
                break;
            }

            for (std::size_t i = begin; i < s.size(); ++i) {
                const std::size_t cur_size = i - begin + 1;
                if (cur_size > best_size) {
                    if (isPalindrome(s, begin, i)) {
                        best_size = cur_size;
                        best_begin = begin;
                        best_end = i;
                    }
                }
            }
        }

        return s.substr(best_begin, best_size);
    }

    bool isPalindrome(const std::string & p_str, std::size_t p_start, std::size_t p_stop) {
        while (p_start < p_stop) {
            if (p_str[p_start++] != p_str[p_stop--]) {
                return false;
            }
        }
        
        return true;
    }
};