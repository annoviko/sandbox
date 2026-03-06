#include <string>


class Solution {
public:
    bool checkOnesSegment(const std::string& s) {
        auto p = s.find('0');
        if (p == std::string::npos) {
            return true;
        }

        p = s.find('1', p + 1);
        return (p == std::string::npos);
    }
};