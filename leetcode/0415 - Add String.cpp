#include <algorithm>
#include <string>


class Solution {
public:
    std::string addStrings(const std::string & num1, const std::string & num2) {
        std::string output;
        output.reserve(num1.size() + 1);

        const std::size_t length = std::max(num1.size(), num2.size());

        std::size_t c = 0;
        for (std::size_t i = 0; i < length; i++) {
            std::size_t result = c;
            result += (i < num1.size()) ? num1[num1.size() - i - 1] - 48 : 0;
            result += (i < num2.size()) ? num2[num2.size() - i - 1] - 48 : 0;

            std::size_t value = (result % 10);
            c = (result - value) / 10;

            output += std::to_string(value);
        }

        if (c != 0) {
            output += std::to_string(c);
        }

        std::reverse(output.begin(), output.end());
        return output;
    }
};