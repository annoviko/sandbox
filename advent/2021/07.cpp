#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>


using crab_locations = std::vector<std::int64_t>;


crab_locations read_input() {
    crab_locations result;

    std::ifstream stream("input.txt");
    for(std::size_t position = 0; !stream.eof();) {
        stream >> position;
        stream.ignore(1);
        result.push_back(position);
    }
 
    return result;
}


enum class consumption {
    constant,
    linear
};


class aligner {
private:
    crab_locations m_locations;

public:
    aligner(const crab_locations& p_locations) : m_locations(p_locations) { }

public:
    std::int64_t run(consumption p_consumption) {
        switch(p_consumption) {
            case consumption::constant: return constant_consumption();
            case consumption::linear: return linear_consumption();
        }

        throw std::exception("unknown consumption");
    }

private:
    std::int64_t constant_consumption() {
        std::sort(m_locations.begin(), m_locations.end(), [](const std::size_t lparam, const std::size_t rparam) {
            return lparam < rparam;
        });

        std::int64_t target = 0;
        const std::int64_t middle = (int64_t) m_locations.size() / 2;

        if (m_locations.size() % 2 != 0) {
            target = (m_locations[middle - 1] + m_locations[middle]) / 2;
        }
        else {
            target = m_locations[middle];
        }

        std::int64_t fuel = 0;
        for (const std::int64_t crab_position : m_locations) {
            fuel += std::abs(crab_position - target);
        }

        return fuel;
    }

    std::int64_t linear_consumption() {
        const std::int64_t furthest_position = *std::max_element(m_locations.begin(), m_locations.end());
        std::vector<std::int64_t> cost(furthest_position + 1, 0);

        for (std::int64_t i = 1; i < (std::int64_t) cost.size(); i++) {
            cost[i] = cost[i - 1] + i;
        }

        std::int64_t minimum_fuel = std::numeric_limits<std::int64_t>::max();
        for (std::int64_t target = 0; target < furthest_position + 1; target++) {
            std::int64_t fuel = 0;
            for (const std::int64_t crab_position : m_locations) {
                const std::int64_t distance = std::abs(target - crab_position);
                fuel += cost[distance];
            }

            //std::cout << target << " : " << fuel << std::endl;
            minimum_fuel = std::min(minimum_fuel, fuel);
        }

        return minimum_fuel;
    }
};


int main() {
    crab_locations crabs = read_input();

    std::cout << "Fuel for crab marine alignment (constant consumption): " << aligner(crabs).run(consumption::constant) << std::endl;
    std::cout << "Fuel for crab marine alignment (linear consumption): " << aligner(crabs).run(consumption::linear) << std::endl;

    return 0;
}