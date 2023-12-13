#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>


std::int64_t count(const std::string& str, const std::size_t str_idx, const std::vector<std::int64_t>& vals, const std::size_t vals_idx);

std::int64_t get_value_from_cache(const std::size_t str_idx, const std::size_t vals_idx);



struct spring {
    std::string scheme;
    std::vector<std::int64_t> values;
};


/* cache result when it was counted [string index][value index] = <possible ways to arrange> */
std::unordered_map<std::int64_t, std::unordered_map<std::int64_t, std::int64_t>> cache;


std::int64_t get_value_from_cache(const std::size_t str_idx, const std::size_t vals_idx) {
    auto iter = cache.find(str_idx);
    if (iter != cache.end()) {
        auto iter_value = iter->second.find(vals_idx);
        if (iter_value != iter->second.end()) {
            return iter_value->second;
        }
    }

    return -1;
}


std::int64_t get_value_from_cache_or_count(const std::string& str, const std::size_t str_idx, const std::vector<std::int64_t>& vals, const std::size_t vals_idx) {
    std::int64_t value_in_cache = get_value_from_cache(str_idx, vals_idx);
    if (value_in_cache == -1) {
        return count(str, str_idx, vals, vals_idx);   /* remove the first element from string and continue */
    }

    return value_in_cache;
}


std::int64_t count(const std::string& str, const std::size_t str_idx, const std::vector<std::int64_t>& vals, const std::size_t vals_idx) {
    if (str_idx == str.size()) {
        if (vals_idx == vals.size()) {
            return 1;   /* valid configuration */
        }
        else {
            return 0;   /* invalid configuration, string is empty, but there are values to proceed */
        }
    }

    if (vals_idx == vals.size()) {
        bool remaining_damaged_springs = false;
        for (std::size_t i = str_idx; i < str.size(); i++) {
            if (str[i] == '#') {
                remaining_damaged_springs = true;
                break;
            }
        }

        if (remaining_damaged_springs) {
            return 0;   /* invalid configuration - we handled all values, but there are still springs that needs to be covered */
        }
        else {
            return 1;   /* valid configuration */
        }
    }

    std::int64_t result = 0;

    std::size_t str_size = str.size() - str_idx;
    std::int64_t cur_val = vals[vals_idx];

    if (str[str_idx] == '.' || str[str_idx] == '?') {
        /* consider it as a '.' - workable spring */
        result += get_value_from_cache_or_count(str, str_idx + 1, vals, vals_idx);
    }

    if (str[str_idx] == '#' || str[str_idx] == '?') {
        /* consider it as a '#' - damaged spring */
        if (cur_val <= str_size) {
            /* there is enough space to consider this configuration */

            bool is_workable_spring_inside = false;
            for (std::size_t i = 0; i < cur_val; i++) {
                if (str[str_idx + i] == '.') {
                    is_workable_spring_inside = true;   /* there is workable spring inside - wrong configuration */
                    break;
                }
            }

            bool is_extended_by_damaged = true;
            if ((cur_val == str_size) || str[cur_val + str_idx] != '#') {
                is_extended_by_damaged = false;      /* there is no broken spring adjacent to this one - wrong configuration */
            }

            if (!is_workable_spring_inside && !is_extended_by_damaged) {
                /* valid configuration, we can place current spring */

                std::int64_t next_str_idx = str_idx + cur_val + 1;  /* capture all objects covered by spring + separator */
                if (next_str_idx > str.size()) {
                    next_str_idx = str.size();
                }

                result += get_value_from_cache_or_count(str, next_str_idx, vals, vals_idx + 1);
            }
        }
    }

    cache[str_idx][vals_idx] = result;

    return result;
}


std::int64_t find_sum_counts(const std::vector<spring>& entries) {
    std::int64_t sum = 0;

    for (const auto& entry : entries) {
        cache.clear();
        sum += count(entry.scheme, 0, entry.values, 0);
    }

    return sum;
}


bool test(const std::string& str, const std::vector<std::int64_t>& vals, std::int64_t expected) {
    cache.clear();
    std::int64_t actual = count(str, 0, vals, 0);
    if (actual != expected) {
        std::cout << "FAIL: " << actual << " (actual) != " << expected << " (expected) - string: '" << str << "'" << std::endl;
        return false;
    }
    else {
        //std::cout << "PASS" << std::endl;
        return true;
    }
}


void test() {
    bool success = true;

    success &= test("???.###", { 1,1,3 }, 1);
    success &= test(".??..??...?##.", { 1,1,3 }, 4);
    success &= test("?#?#?#?#?#?#?#?", { 1,3,1,6 }, 1);
    success &= test("????.#...#...", { 4,1,1 }, 1);
    success &= test("????.######..#####.", { 1,6,5 }, 4);
    success &= test("?###????????", { 3,2,1 }, 10);

    success &= test("?#?#?.?#.?", { 1,1,2 }, 1);
    success &= test(".?#??.???#?", { 2,1,1 }, 5);
    success &= test("??.##???#??????", { 1,6,2 }, 8);
    success &= test("??#???.#??.#.", { 3,2,1 }, 3);

    if (success) {
        std::cout << "All tests PASSED" << std::endl;
    }
}


std::vector<spring> read_input() {
    std::fstream stream("input.txt");

    std::vector<spring> result;
    for (std::string line; std::getline(stream, line);) {
        spring entry;

        std::size_t p = line.find(' ');
        entry.scheme = line.substr(0, p);

        std::int64_t value = 0;
        for (auto i = p + 1; i < line.size(); i++) {
            if (line[i] == ',') {
                entry.values.push_back(value);
                value = 0;
            }
            else {
                value *= 10;
                value += line[i] - '0';
            }
        }

        entry.values.push_back(value);

        result.push_back(std::move(entry));
    }

    return result;
}


int main() {
#if 0
    test();
#else
    auto input = read_input();

    std::cout << "The number of valid configurations: " << find_sum_counts(input) << std::endl;

    for (int k = 0; k < input.size(); k++) {
        auto& e = input[k];

        std::string unfolded_scheme = e.scheme;
        std::vector<std::int64_t> unfolded_values = e.values;

        for (int i = 0; i < 4; i++) {
            unfolded_scheme.push_back('?');
            unfolded_scheme += e.scheme;

            unfolded_values.insert(unfolded_values.end(), e.values.begin(), e.values.end());
        }

        input[k] = { unfolded_scheme, unfolded_values };
    }

    std::cout << "The number of valid configurations (unfolded): " << find_sum_counts(input) << std::endl;
#endif

    return 0;
}