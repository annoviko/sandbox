#include <string>
#include <vector>
#include <cmath>

class Solution {
public:
    bool hasAllCodes(const std::string& s, int k) {
        if (s.size() < k) {
            return false;
        }

        std::vector<bool> found_values(std::pow(2, k), false);

        std::string pattern = s.substr(0, k);
        std::uint32_t cur_value = 0;
        for (int i = 0; i < pattern.size(); i++) {
            cur_value = (cur_value << 1) + pattern[i] - '0';
        }

        found_values[cur_value] = true;
        const std::uint32_t clean_mask = ~(1 << (k - 1));

        for (int i = k; i < s.size(); i++) {
            cur_value = clean_mask & cur_value;
            cur_value = (cur_value << 1);
            cur_value += (s[i] - '0');
            found_values[cur_value] = true;
        }

        for (auto is_found : found_values) {
            if (!is_found) {
                return false;
            }
        }

        return true;
    }
};