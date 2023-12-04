#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::uint64_t extract_first_calibration_value(const std::string& line) {
    std::size_t lpos = line.find_first_of("0123456789");
    return line[lpos] - '0';
}


std::uint64_t extract_last_calibration_value(const std::string& line) {
    std::size_t rpos = line.find_last_of("0123456789");
    return line[rpos] - '0';
}


std::uint64_t calibration_values_from_digits() {
    std::uint64_t sum = 0;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        auto lvalue = extract_first_calibration_value(line);
        auto rvalue = extract_last_calibration_value(line);

        sum += lvalue * 10 + rvalue;
    }

    return sum;
}


const std::vector<std::string> str_to_digit = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };


std::string replace(const std::string& line, const bool replace_from_begin) {
    std::string result = line;

    std::size_t pos = -1;
    std::size_t index = -1;

    for (int i = 0; i < str_to_digit.size(); i++) {
        std::size_t pos_candidate = replace_from_begin ? line.find(str_to_digit[i]) : line.rfind(str_to_digit[i]);
        if (pos_candidate == std::string::npos) {
            continue;
        }

        if ((index == -1) ||
            (replace_from_begin && (pos_candidate < pos)) ||
            (!replace_from_begin && (pos_candidate > pos)))
        {
            pos = pos_candidate;
            index = i;
        }
    }

    if (pos != -1) {
        result.replace(pos, str_to_digit[index].size(), std::string(1, '1' + index));
    }

    return result;
}



std::string replace_front(const std::string& line) {
    return replace(line, true);
}


std::string replace_end(const std::string& line) {
    return replace(line, false);
}



std::uint64_t calibration_values_from_everything() {
    std::uint64_t sum = 0;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        auto lvalue = extract_first_calibration_value(replace_front(line));
        auto rvalue = extract_last_calibration_value(replace_end(line));

        sum += lvalue * 10 + rvalue;
    }

    return sum;
}


int main() {
    std::cout << "The sum of all of the calibration values (digits only): " << calibration_values_from_digits() << std::endl;
    std::cout << "The sum of all of the calibration values: " << calibration_values_from_everything() << std::endl;

    return 0;
}