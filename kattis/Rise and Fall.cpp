#include <cstdint>
#include <iostream>
#include <string>


class solution {
public:
    std::string rise_and_fall(const std::string& p_value) {
        std::string result;
        result.push_back(p_value[0]);

        std::uint64_t i = 1;
        for (; i < p_value.size(); i++) {
            if (result.back() <= p_value[i]) {
                result.push_back(p_value[i]);
            }
            else {
                break;
            }
        }

        if (i >= p_value.size()) {
            return result; /* fall part is empty */
        }

        result.push_back(p_value[i]);
        for (i = i + 1; i < p_value.size(); i++) {
            if (result.back() >= p_value[i]) {
                result.push_back(p_value[i]);
            }
            else {
                std::uint64_t positions_to_fill = p_value.size() - i;
                std::string filler = std::string(positions_to_fill, result.back());
                result += filler;
                break;
            }
        }

        return result;
    }
};



#include <unordered_map>

void test() {
    std::unordered_map<std::string, std::string> tests = {
        { "29041", "29000" },
        { "56577", "56555" },
        { "4739475682", "4733333333" },
        { "98765554", "98765554" },
        { "1234567", "1234567" },
        { "5", "5" },
        { "111111111111111111111111111112222222222222222223333333333333333333333333333322222222222222222222222222", "111111111111111111111111111112222222222222222223333333333333333333333333333322222222222222222222222222" }
    };

    for (auto& t : tests) {
        auto actual = solution().rise_and_fall(t.first);
        if (actual != t.second) {
            std::cout << "[FAIL] actual: '" << actual << "', expected: '" << t.second << "'" << std::endl;
        }
        else {
            std::cout << "[PASS]" << std::endl;
        }
    }
}

#if 0
    #define TEST
#endif

int main() {
#if defined(TEST)
    test();
#else
    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string value;
        std::cin >> value;

        std::string result = solution().rise_and_fall(value);
        std::cout << result << std::endl;
    }
#endif
    return 0;
}