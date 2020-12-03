class Solution {
public:
#ifdef PURE_STRING_SOLUTION
    std::string addBinary(std::string a, std::string b) {
        std::string result = "";
        bool carry = false;

        for (int ai = a.size() - 1, bi = b.size() - 1; ai >= 0 && bi >= 0; ai--, bi--) {
            if (a[ai] == b[bi]) {
                result.push_back(carry ? '1' : '0');
                carry = (a[ai] == '1');
            }
            else {
                result.push_back(carry ? '0' : '1');
            }
        }

        std::string & remain = a.size() > b.size() ? a : b;
        for (int i = remain.size() - result.size() - 1; i >= 0; i--) {
            if (remain[i] == '1') {
                result.push_back(carry ? '0' : '1');
            }
            else {
                result.push_back(carry ? '1' : '0');
                carry = false;
            }
        }

        if (carry) { result.push_back('1'); }

        std::reverse(result.begin(), result.end());
        return result;
    }
#else
    std::string addBinary(std::string a, std::string b) {
        std::string result;

        int ai = a.size() - 1;
        int bi = b.size() - 1;
        int carry = 0;

        while (ai >= 0 || bi >= 0) {
            int value = carry;
            value += (ai >= 0) ? a[ai--] - '0' : 0;
            value += (bi >= 0) ? b[bi--] - '0' : 0;

            result.push_back(value % 2 == 0 ? '0' : '1');
            carry = value >> 1;
        }

        if (carry > 0) { result.push_back('1'); }

        std::reverse(result.begin(), result.end());
        return result;
    }
#endif
};