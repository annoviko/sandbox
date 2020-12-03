class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) {
            return "";
        }
        
        std::string prefix = strs[0];
        for (std::size_t i = 1; (i < strs.size()) && (!prefix.empty()); i++) {
            if (prefix.size() > strs[i].size()) {
                prefix.erase(strs[i].size());
            }
            
            for (std::size_t j = 0; j < prefix.size(); j++) {
                if (prefix[j] != strs[i][j]) {
                    prefix.erase(j);
                    continue;
                }
            }
        }
        
        return prefix;
    }
};