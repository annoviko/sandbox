class Solution {
public:
    int lengthOfLastWord(const std::string & s) {
        if (s.empty()) { return 0; };

        int length = 0;

        auto it = s.rbegin();
        for (; it != s.rend() && *it == ' '; it++);

        for (; it != s.rend() && *it != ' '; it++)
        {
            length++;
        }

        return length;
    }
};