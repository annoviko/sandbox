#include <cmath>
#include <iostream>
#include <list>
#include <string>


std::size_t babilonian_to_decimal(const std::string & p_code) {
    std::list<std::size_t> digits;

    std::size_t lborder = 0;
    std::size_t rborder = p_code.find(",");

    while (rborder != std::string::npos) {
        const std::string babilonian_digit = p_code.substr(lborder, rborder - lborder);
        if (babilonian_digit.empty()) {
            digits.push_back(0);
        }
        else {
            digits.push_back(std::stoull(babilonian_digit));
        }

        lborder = rborder + 1;
        rborder = p_code.find(",", lborder);
    }

    const std::string babilonian_digit = p_code.substr(lborder);
    if (babilonian_digit.empty()) {
        digits.push_back(0);
    }
    else {
        digits.push_back(std::stoull(p_code.substr(lborder)));
    }

    std::size_t result = 0;
    auto iter = digits.begin();
    for (int degree = static_cast<int>(digits.size()) - 1; degree >= 0; degree--) {
        result += *iter * std::pow(60, degree);
        iter++;
    }

    return result;
}


int main() {
    std::size_t n = 0;
    std::cin >> n;

    for (std::size_t i = 0; i < n; i++) {
        std::string code;
        std::cin >> code;
        std::cout << babilonian_to_decimal(code) << std::endl;
    }

    return 0;
}
