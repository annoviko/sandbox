#include <cmath>
#include <iostream>
#include <string>
#include <vector>


const std::int64_t modulo = 1000000007;


class solution {
private:
    int n = 0;
    int cnt = 0;

public:
    solution(int n_) : n(n_) { }

    std::int64_t eval(const bool is_add) {
        std::int64_t result = -1;

        for (std::string str_val = get(); !str_val.empty() && str_val != ")"; str_val = get()) {
            std::uint64_t rvalue = 0;

            if (str_val == ")") {
                return result;
            }
            else if (str_val == "(") {
                rvalue = eval(!is_add); /* already with modulo */
            }
            else {
                rvalue = std::stoll(str_val) % modulo;
            }

            if (result < 0) {
                result = rvalue;
            }
            else {
                if (is_add) {
                    result += rvalue;
                }
                else {
                    result *= rvalue;
                }

                result %= modulo;
            }
        }

        return result;  /* already with modulo */
    }

private:
    std::string get() {
        if (cnt < n) {
            cnt++;
            std::string value;
            std::cin >> value;
            return value;
        }

        return std::string();
    }
};


int main() {
    int n;
    std::cin >> n;

    std::cout << solution(n).eval(true) << std::endl;

    return 0;
}
