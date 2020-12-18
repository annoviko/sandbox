#include <fstream>
#include <iostream>
#include <unordered_map>


class memory_game {
private:
    std::vector<std::size_t> m_values;
    std::vector<std::size_t> m_counter;		/* faster than std::unordered_map */

public:
    std::size_t play(const std::vector<std::size_t> & p_initial, const std::size_t p_step) {
        m_values.reserve(p_step);
        m_counter.resize(p_step, 0);

        initialize(p_initial);

        for (std::size_t i = m_values.size(); i < p_step; i++) {
            const std::size_t current_value = m_values.back();
            const std::size_t counter_value = m_counter[current_value];
            if (counter_value == 0) {
                m_values.push_back(0);
            }
            else {
                m_values.push_back(i - counter_value);
            }

            m_counter[current_value] = i;
        }

        return m_values.back();
    }

private:
    void initialize(const std::vector<std::size_t> & p_initial) {
        m_values = p_initial;

        for (std::size_t i = 0; i < m_values.size() - 1; i++) {
            m_counter[m_values[i]] = i + 1;
        }
    }
};


int main() {
    std::cout << "The 2020th number: " << memory_game().play({ 12, 1, 16, 3, 11, 0 }, 2020) << std::endl;
    std::cout << "The 30000000th number: " << memory_game().play({ 12, 1, 16, 3, 11, 0 }, 30000000) << std::endl;

    return 0;
}
