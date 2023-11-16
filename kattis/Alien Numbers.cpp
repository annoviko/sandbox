// https://open.kattis.com/problems/aliennumbers

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>


std::string convert(const std::string& p_value, const std::string& p_src, const std::string& p_dst) {
    int dec_value = 0;

    std::unordered_map<char, char> dict;
    for (int i = 0; i < p_src.size(); i++) {
        dict[p_src[i]] = i;
    }

    int base = p_src.size();
    for (int i = 0; i < p_value.size(); i++) {
        const char digit = p_value[i];

        int dec_digit = dict[digit];
        dec_value *= base;
        dec_value += dec_digit;
    }

    dict.clear();
    for (int i = 0; i < p_dst.size(); i++) {
        dict[i] = p_dst[i];
    }

    std::string result;

    base = p_dst.size();
    while (dec_value != 0) {
        int rem = dec_value % base;
        dec_value /= base;

        result.push_back(dict[rem]);
    }

    std::reverse(result.begin(), result.end());

    return result;
}


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string value, src, dst;
        std::cin >> value >> src >> dst;

        std::cout << "Case #" << i + 1 << ": " << convert(value, src, dst) << std::endl;
    }

    return 0;
}