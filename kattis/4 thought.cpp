#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


std::unordered_map<std::int64_t, std::string> precompute() {
    std::unordered_map<std::int64_t, std::string> result;

    const std::string operations = "+-*/";

    for (char op1: operations) {
        for (char op2 : operations) {
            for (char op3 : operations) {
                std::string order = { op1, op2, op3 };

                std::string expr = "4 " + std::string(1, op1) + " 4 " + std::string(1, op2) + " 4 " + std::string(1, op3) + " 4 = ";

                std::vector<std::int64_t> values = { 4, 4, 4, 4 };

                for (int i = 0; i < order.size(); i++) {
                    if (order[i] == '*' || order[i] == '/') {
                        if (order[i] == '*') {
                            values[i] *= values[i + 1];
                        }
                        else {
                            values[i] /= values[i + 1];
                        }

                        values.erase(values.begin() + i + 1);
                        order.erase(order.begin() + i);
                        i--;
                    }
                }

                std::int64_t value = *(values.begin());
                for (int i = 0; i < order.size(); i++) {
                    if (order[i] == '+') {
                        value += values[i + 1];
                    }
                    else {
                        value -= values[i + 1];
                    }
                }

                expr += std::to_string(value);
                result[value] = expr;
            }
        }
    }

    return result;
}



int main() {
    auto dict = precompute();

    int m = 0;
    std::cin >> m;

    for (int i = 0; i < m; i++) {
        int n = 0;
        std::cin >> n;

        auto iter = dict.find(n);
        if (iter == dict.end()) {
            std::cout << "no solution" << std::endl;
        }
        else {
            std::cout << iter->second << std::endl;
        }
    }

    return 0;
}