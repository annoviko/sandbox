#include <cctype>
#include <string>

class Solution {
public:
    bool isPalindrome(const std::string & s) {
        std::string canonical;
        for (auto sym : s) {
            if (std::isalpha(sym) || std::isdigit(sym)) {
                canonical.push_back(std::tolower(sym));
            }
        }
        
        for (std::size_t i = 0; i < canonical.size(); i++) {
            if (canonical[i] != canonical[canonical.size() - i - 1]) {
                return false;
            }
        }
        
        return true;
    }
};