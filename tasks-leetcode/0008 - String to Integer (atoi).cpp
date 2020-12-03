class Solution {
public:
    int myAtoi(std::string & str) {
        std::string pure_number;
        bool negative = false;
        bool digit_expected = false;
        for (std::size_t i = 0; i < str.size(); i++) {
            if (isdigit(str[i])) {
                digit_expected = true;
                if (pure_number.empty() && (i > 0) && ((str[i - 1] == '-') || str[i - 1] == '+')) {
                    if (i - 1 > 0 && str[i - 2] != ' ') {
                        break;
                    }

                    if (str[i - 1] == '-')
                        negative = true;
                }

                pure_number += str[i];
            }
            else if (digit_expected) { break; }
            else {
                if (str[i] == '-' || str[i] == '+') { 
                    digit_expected = true;
                    continue; 
                }
                
                if (str[i] == ' ' && pure_number.empty()) {
                    continue;
                }

                break;
            }
        }

        int result = 0;
        for (int i = pure_number.size() - 1; i >= 0; i--) {
            int degree = pure_number.size() - i - 1;
            int digit = static_cast<int>(pure_number[i] - '0');
            int diff = digit * std::pow(10, degree);

            if (!negative && (INT_MAX - diff < result)) {
                result = INT_MAX;
                break;
            }
            else if (negative && (INT_MIN + diff > -result)) {
                result = INT_MIN;
                break;
            }

            result += diff;
        }

        if (negative) {
            result *= -1;
        }

        return result;
    }
};