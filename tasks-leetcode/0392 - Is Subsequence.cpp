class Solution {
public:
    bool isSubsequence(string s, string t) {
        auto iter_pattern = s.begin();
        auto iter_text = t.begin();
        
        for (; iter_pattern != s.end() && iter_text != t.end(); iter_text++) {
            if (*iter_pattern == *iter_text) {
                iter_pattern++;    
            }
        }
        
        return iter_pattern == s.end();
    }
};