class Solution {
public:
    bool isAnagram(const std::string & s, const std::string & t) {
        std::vector<int> counter(26);
        
        for (char sym : s) {
            counter[sym - 'a']++;
        }
        
        for (char sym : t) {
            counter[sym - 'a']--;
        }
        
        for (int sum : counter) {
            if (sum != 0) {
                return false;
            }
        }
        
        return true;
    }
};