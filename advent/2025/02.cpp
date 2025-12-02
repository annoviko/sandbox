#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


enum class method_t {
    HALF,
    ALL
};


std::uint64_t sum_invalid_ids_in_range_1(const std::string& p_value, const std::string& p_limit) {
    if (p_value.size() % 2 != 0 && p_value.size() == p_limit.size()) {
        return 0;
    }

    std::uint64_t result = 0;

    std::string part;

    if (p_value.size() % 2 == 0) {
        std::uint64_t left_part = std::stoull(p_value.substr(0, p_value.size() / 2));
        std::uint64_t right_part = std::stoull(p_value.substr(p_value.size() / 2));

        part = left_part < right_part ? std::to_string(left_part + 1) : std::to_string(left_part);
    }
    else {
        part = std::string("1") + std::string(p_value.size() / 2, '0');
    }

    std::string cur = part + part;

    const auto limit = std::stoull(p_limit);
    while (std::stoull(cur) <= limit) {
        result += std::stoull(cur);

        part = std::to_string(std::stoull(part) + 1);
        cur = part + part;
    }

    return result;
}


std::uint64_t create_value_from_part(const std::string& part, int cur_len) {
    std::string cur_str;
    for (int i = 0; i < cur_len; i += part.size()) {
        cur_str += part;
    }

    return std::stoull(cur_str);
}


std::vector<std::uint64_t> fit(const std::string& part, const std::string& str_begin, const std::string& str_end) {
    std::vector<std::uint64_t> results;

    const std::uint64_t begin = std::stoull(str_begin), end = std::stoull(str_end);
    for (int len = str_begin.size(); len <= str_end.size(); len++) {
        if (len == 1) {
            continue;   /* invalid ID has at least 2 digits */
        }

        if (len % part.size() != 0) {
            continue;
        }

        auto cur = create_value_from_part(part, len);
        if (cur >= begin && cur <= end) {
            results.push_back(cur);
        }
    }

    return results;
}

std::uint64_t ITERATION_COUNTER = 0;

std::uint64_t sum_invalid_ids_in_range_2(const std::string& p_value, const std::string& p_limit) {
    std::string part = (p_value.size() < p_limit.size()) ? "1" : std::string(1, p_value[0]);
    std::uint64_t part_size = part.size();

    /* to prevent calling substr every time */
    std::vector<std::uint64_t> begin_fractions, end_fractions;
    for (int i = 1; i <= p_value.size(); i++) {
        begin_fractions.push_back(std::stoull(p_value.substr(0, i)));
    }
    for (int i = 1; i <= p_limit.size(); i++) {
        end_fractions.push_back(std::stoull(p_limit.substr(0, i)));
    }

    std::unordered_set<std::uint64_t> unique;

    while (part_size <= (p_limit.size() >> 1)) {
        ITERATION_COUNTER++;

        const auto values = fit(part, p_value, p_limit);
        for (const auto v : values) {
            unique.insert(v);
        }

        std::uint64_t int_part = std::stoull(part) + 1;
        part_size = std::to_string(int_part).size();

        if (p_value.size() == p_limit.size()) {
            const std::uint64_t cur_fraction_begin = begin_fractions[part_size - 1];
            const std::uint64_t cur_fraction_end = end_fractions[part_size - 1];

            /* skips if common fraction exceeded */
            if (int_part > cur_fraction_begin && int_part > cur_fraction_end) {
                int_part = begin_fractions[part_size];
            }
        }
        else if (part_size > 1) {
            if (p_value.size() % part_size == 0) {
                const std::uint64_t cur_fraction_begin = begin_fractions[part_size - 1];

                /* skip fractions that are smaller, we fit only 'from' value */
                if (int_part < cur_fraction_begin) {
                    int_part = begin_fractions[part_size - 1];
                }
            }
            else if (p_limit.size() % part_size == 0) {
                const std::uint64_t cur_fraction_end = end_fractions[part_size - 1];

                /* skip fractions that are bigger, we fit only 'to' value */
                if (int_part > cur_fraction_end && part_size < begin_fractions.size()) {
                    int_part = std::stoull("1" + std::string(part_size, '0'));
                }
            }
        }

        part = std::to_string(int_part);
        part_size = part.size();
    }

    std::uint64_t result = 0;
    for (const auto v : unique) {
        result += v;
    }

    return result;
}


std::uint64_t sum_invalid_ids(const std::vector<std::pair<std::string, std::string>>& p_ranges, const method_t p_method) {
    std::uint64_t sum = 0;

    for (const auto& pair : p_ranges) {
        switch (p_method) {
        case method_t::HALF:
            sum += sum_invalid_ids_in_range_1(pair.first, pair.second);
            break;

        case method_t::ALL:
            sum += sum_invalid_ids_in_range_2(pair.first, pair.second);
            break;
        }
    }

    return sum;
}


std::vector<std::pair<std::string, std::string>> read_input() {
    std::fstream stream("input.txt");

    std::vector<std::pair<std::string, std::string>> result;
    for (std::string range; std::getline(stream, range, ',');) {
        auto pos = range.find('-');

        auto from = range.substr(0, pos);
        auto to = range.substr(pos + 1);

        result.push_back({ from, to });
    }

    return result;
}


int main() {
    auto input = read_input();

    std::cout << "Sum of invalid IDs: " << sum_invalid_ids(input, method_t::HALF) << std::endl;
    std::cout << "Sum of invalid IDs (new rules): " << sum_invalid_ids(input, method_t::ALL) << std::endl;

    std::cout << std::endl;
    std::cout << "--------------------------------" << std::string(std::to_string(ITERATION_COUNTER).size(), '-') << std::endl;
    std::cout << "[Statistics] Iteration Counter: " << ITERATION_COUNTER << std::endl;

    return 0;
}