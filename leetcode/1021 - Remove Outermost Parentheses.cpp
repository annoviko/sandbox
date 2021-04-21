class Solution {
public:
    std::string removeOuterParentheses(const std::string & s) {
        std::size_t counter = 0;

        std::string result;
        result.reserve(s.size());

        for (const auto symbol : s) {
            switch (symbol) {
            case '(':
                if (counter != 0) {
                    result.push_back(symbol);
                }

                counter++;
                break;

            case ')':
                counter--;

                if (counter != 0) {
                    result.push_back(symbol);
                }

                break;
            }
        }

        return result;
    }
};
