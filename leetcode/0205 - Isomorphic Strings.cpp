class Solution {
public:
    bool isIsomorphic(const std::string & s, const std::string & t) {
        std::vector<std::size_t> s_to_t(128, INT_MAX);
        std::vector<std::size_t> t_to_s(128, INT_MAX);

        for (std::size_t i = 0; i < s.size(); i++) {
            if ((s_to_t[s[i]] == INT_MAX) && (t_to_s[t[i]] == INT_MAX)) {
                s_to_t[s[i]] = t[i];
                t_to_s[t[i]] = s[i];
            }

            if (s_to_t[s[i]] != t[i]) {
                return false;
            }
        }

        return true;
    }
};