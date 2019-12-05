#include <iostream>
#include <string>


enum class thought {
    only_facts,
    elfs_remember_details
};


bool is_meet_criteria(const long p_value, const thought p_method) {
    if ((p_value < 100000) || (p_value > 999999)) {
        return false;
    }

    std::size_t amount_adjacent_digits = 1;
    bool has_adjacent_pair = false;
    bool has_pure_pair = false;

    const std::string string_value = std::to_string(p_value);
    for (std::size_t i = 1; i < string_value.size(); i++) {
        if (string_value[i - 1] > string_value[i]) {
            return false;
        }
        else if (string_value[i - 1] == string_value[i]) {
            amount_adjacent_digits++;
        }
        else {
            if (amount_adjacent_digits > 1) {
                has_adjacent_pair = true;
                if (amount_adjacent_digits == 2) {
                    has_pure_pair = true;
                }
            }
            
            amount_adjacent_digits = 1;
        }
    }

    if (amount_adjacent_digits > 1) {
        has_adjacent_pair = true;
        if (amount_adjacent_digits == 2) {
            has_pure_pair = true;
        }
    }

    if (p_method == thought::elfs_remember_details) {
        if (!has_pure_pair) {
            return false;
        }
    }

    return has_adjacent_pair;
}


std::size_t count_password_combinations(const long p_start, const long p_end, const thought p_method) {
    std::size_t result = 0;
    for (long i = p_start; i <= p_end; i++) {
        if (is_meet_criteria(i, p_method)) {
            result++;
        }
    }

    return result;
}


int main() {
    std::cout << "Password combinations #1: " << count_password_combinations(254032, 789860, thought::only_facts) << std::endl;
    std::cout << "Password combinations #2: " << count_password_combinations(254032, 789860, thought::elfs_remember_details) << std::endl;
    return 0;
}
