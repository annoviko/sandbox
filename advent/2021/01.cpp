#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>


using measurement = std::vector<std::size_t>;


measurement read_input() {
    measurement result;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line); ) {
        result.push_back(std::stoul(line));
    }

    return result;
}


class depth_increase_counter {
private:
    constexpr static std::size_t INFINITY_DEPTH = std::numeric_limits<std::size_t>::max();

public:
    static std::size_t calculate(const measurement& p_measurement) {
        return calculate_with_slide_window(p_measurement, 1);
    }

    static std::size_t calculate_with_slide_window(const measurement& p_measurement, const std::size_t p_window_length) {
        std::size_t result = 0;

        std::size_t current_depth = 0;
        for (std::size_t i = 0; (i < p_measurement[i]) && (i < p_window_length); i++) {
            current_depth += p_measurement[i];
        }

        for (std::size_t i = p_window_length; i < p_measurement.size(); i++) {
            const std::size_t next_depth = current_depth + p_measurement[i] - p_measurement[i - p_window_length];
            if (current_depth < next_depth) {
                result++;
            }

            current_depth = next_depth;
        }

        return result;
    }
};


int main() {
    const measurement input = read_input();

    std::cout << "Depth increase counter: " << depth_increase_counter::calculate(input) << std::endl;
    std::cout << "Depth increase counter (slide window approach): " << depth_increase_counter::calculate_with_slide_window(input, 3) << std::endl;

    return 0;
}
