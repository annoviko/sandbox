#include <string>

class Solution {
public:
    int numSteps(std::string s) {
        int steps = 0;
        for (; s != "1"; steps++) {
            if (s.back() == '0') {
                s.pop_back();
            }
            else {
                int c = 1;
                for (int i = s.size() - 1; i >= 0 && c == 1; i--) {
                    if (s[i] == '1') {
                        s[i] = '0';
                    }
                    else {
                        s[i] = '1';
                        c = 0;
                    }
                }

                if (c == 1) {
                    s = "1" + s;
                }
            }
        }

        return steps;
    }
};
