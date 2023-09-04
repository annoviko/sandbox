#include <iomanip>
#include <iostream>
#include <string>


int main() {
    int whitespace = 0, lowercase = 0, uppercase = 0, symbols = 0;

    std::string input;
    std::cin >> input;

    for (const auto val : input) {
        if (val == '_') {
            whitespace++;
        }
        else if (val >= 'a' && val <= 'z') {
            lowercase++;
        }
        else if (val >= 'A' && val <= 'Z') {
            uppercase++;
        }
        else {
            symbols++;
        }
    }

    std::cout << std::setprecision(15) << (static_cast<double>(whitespace) / static_cast<double>(input.size())) << std::endl;
    std::cout << std::setprecision(15) << (static_cast<double>(lowercase) / static_cast<double>(input.size())) << std::endl;
    std::cout << std::setprecision(15) << (static_cast<double>(uppercase) / static_cast<double>(input.size())) << std::endl;
    std::cout << std::setprecision(15) << (static_cast<double>(symbols) / static_cast<double>(input.size())) << std::endl;

    return 0;
}