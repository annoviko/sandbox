#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


using adapter_sequence = std::vector<std::size_t>;


adapter_sequence read_adapters(const std::string & p_file) {
    std::ifstream stream(p_file);

    adapter_sequence result;

    for (std::string joltage; std::getline(stream, joltage);) {
        result.push_back(std::stoull(joltage));
    }

    return result;
}


adapter_sequence sort_adapters(const adapter_sequence & p_adapters) {
    adapter_sequence result(p_adapters.size(), 0);

    const std::size_t maximum = *std::max_element(p_adapters.begin(), p_adapters.end());

    std::vector<std::size_t> counter(maximum + 1, 0);
    for (auto joltage : p_adapters) {
        counter[joltage]++;
    }

    std::size_t index_element = 0;
    for (std::size_t i = 0; i < counter.size(); i++) {
        while (counter[i] > 0) {
            result[index_element] = i;
            index_element++;
            counter[i]--;
        }
    }

    return result;
}


std::size_t calculate_joilt_difference(const adapter_sequence & p_sorted_adapters) {
    std::size_t previous_joltage = 0;
    std::size_t delta_1_counter = 0;
    std::size_t delta_2_counter = 0;
    std::size_t delta_3_counter = 1;
    
    for (auto current_joltage : p_sorted_adapters) {
        const std::size_t difference = current_joltage - previous_joltage;
        if (difference == 1) {
            delta_1_counter++;
        }
        else if (difference == 2) {
            delta_2_counter++;
        }
        else if (difference == 3) {
            delta_3_counter++;
        }

        previous_joltage = current_joltage;
    }

    return delta_1_counter * delta_3_counter;
}


std::size_t calculate_amount_connections_ways(const adapter_sequence & p_sorted_adapters) {
    adapter_sequence dp(p_sorted_adapters.back() + 1, 0);
    dp[0] = 1;

    const adapter_sequence difference = { 1, 2, 3 };

    for (std::size_t i = 0; i < p_sorted_adapters.size(); i++) {
        std::size_t joltage = p_sorted_adapters[i];

        std::size_t ways = 0;
        for (const auto delta : difference) {
            if (joltage >= delta) {
                ways += dp[joltage - delta];
            }
        }

        dp[joltage] = ways;
    }

    return dp.back();
}



int main() {
    auto adapters = sort_adapters(read_adapters("input.txt"));

    std::cout << "The joltage difference between 1-jolt and 3-jolt: " << calculate_joilt_difference(adapters) << std::endl;
    std::cout << "The amount of ways to build charge adapter: " << calculate_amount_connections_ways(adapters) << std::endl;

    return 0;
}
