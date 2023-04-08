class Solution {
public:
    int longestPalindrome(const std::string& s) {
        char vals[256];
        std::memset(&vals, 0x00, sizeof(vals));

        int counter = 0;
        for (auto c : s) {
            if (vals[c] == 0) {
                vals[c]++;
            }
            else if (vals[c] == 1) {
                vals[c] = 0;
                counter += 2;
            }
        }

        if (counter < s.size()) {
            counter++;
        }

        return counter;
    }
};
