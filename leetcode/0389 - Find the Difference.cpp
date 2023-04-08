class Solution {
public:
    char findTheDifference(const std::string& s, const std::string& t) {
        int value = 0;
        for (int i = 0; i < s.size(); i++) {
            value += (t[i] - s[i]);
        }

        value += t.back();
        return (char) value;
    }
};