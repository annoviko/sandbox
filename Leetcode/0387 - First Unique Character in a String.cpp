class Solution {
public:
    int firstUniqChar(std::string & s) {
        std::unordered_map<char, std::size_t> freq;
        for (const auto sym : s) {
            freq[sym]++;
        }
        
        for (std::size_t i = 0; i < s.size(); i++) {
            if (freq[s[i]] == 1) {
                return i;
            }
        }
        
        return -1;
    }
};