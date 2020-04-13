#include <string>


class Solution {
public:
    bool backspaceCompare(const std::string & S, const std::string & T) {
        long long i = S.size() - 1, j = T.size() - 1;

        // O(N) - complexity
        // O(1) - space
        for (long long i = S.size() - 1, j = T.size() - 1;;
            i--, j--)
        {
            i = get_next(S, i);
            j = get_next(T, j);

            if (i < 0 && j < 0) { return true; }
            if (i >= 0 && j >= 0) {
                if (S[i] != T[j]) { return false; }
            }
            else {
                return false;
            }
        }

        return true;
    }

private:
    long long get_next(const std::string & p_line, const std::size_t p_cur) {
        long long ignore = 0;
        long long index = p_cur;

        while (index >= 0 && p_line[index] == '#') {
            while (index >= 0 && p_line[index] == '#') {
                ignore++;
                index--;
            }

            while (index >= 0 && ignore > 0) {
                if (p_line[index] == '#') {
                    break;
                }

                ignore--;
                index--;
            }
        }

        return index;
    }
};