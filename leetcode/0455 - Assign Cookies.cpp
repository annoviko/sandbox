class Solution {
public:
    int findContentChildren(std::vector<int>& g, std::vector<int>& s) {
        std::sort(g.begin(), g.end());
        std::sort(s.begin(), s.end());
        
        int result = 0;
        for (int g_i = 0, s_i = 0; (g_i < g.size()) && (s_i < s.size()); s_i++) {
            if (s[s_i] >= g[g_i]) {
                result++;
                g_i++;
            }
        }
        
        return result;
    }
};