class Solution {
public:
    bool isSubsequence(std::string s, std::string t) {
        if (t.size() < s.size()) {
            return false;
        }

        std::size_t position = 0;
        for (const auto symbol : s) {
            if (position >= t.size()) {
                return false;
            }

            position = t.find(symbol, position);
            if (position == std::string::npos) {
                return false;
            }

            position++;
        }

        return true;
    }
};
