class Solution {
public
    void reverseString(vectorchar& s) {
        for (int i = 0, j = s.size() - 1; i  j; i++, j--) {
            stdswap(s[i], s[j]);
        }
    }
};