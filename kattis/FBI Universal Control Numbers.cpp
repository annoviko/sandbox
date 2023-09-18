#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    int n = 0;
    std::cin >> n;

    const std::unordered_map<char, char> dict_conv = {
        { 'B', '8' },
        { 'G', 'C' },
        { 'I', '1' },
        { 'O', '0' },
        { 'Q', '0' },
        { 'S', '5' },
        { 'U', 'V' },
        { 'Y', 'V' },
        { 'Z', '2' }
    };

    std::string values = "0123456789ACDEFHJKLMNPRTVWX";
    std::unordered_map<char, int> dict_value;
    for (int i = 0; i < values.size(); i++) {
        dict_value[values[i]] = i;
    }

    std::vector<std::int64_t> coefs = { 2, 4, 5, 7, 8, 10, 11, 13 };

    std::int64_t check_value = 0;
    int k;

    for (int i = 0; i < n; i++) {
        std::cin >> k;

        std::uint64_t check_code = 0;
        std::uint64_t decoded_value = 0;
        for (int j = 0; j < 9; j++) {
            char sym = 0;
            std::cin >> sym;

            auto iter = dict_conv.find(sym);
            if (iter != dict_conv.end()) {
                sym = iter->second;
            }

            std::int64_t particle = dict_value[sym];
            if (j < coefs.size()) {
                decoded_value += std::pow(27, 7 - j) * particle;
                check_code += coefs[j] * particle;
            }
            else {
                check_value = particle;
            }
        }

        check_code = check_code % 27;
        if (check_code != check_value) {
            std::cout << i + 1 << " Invalid" << std::endl;
        }
        else {
            std::cout << i + 1 << ' ' << decoded_value << std::endl;
        }
    }

    return 0;
}