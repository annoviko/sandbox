#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


using xmas_sequence = std::vector<std::size_t>;


xmas_sequence read_data() {
    std::ifstream stream("input.txt");
    xmas_sequence result;

    for (std::string value; std::getline(stream, value); ) {
        result.push_back(std::stoull(value));
    }

    return result;
}


class fault_detector {
public:
    struct report {
        std::size_t invalid_value       = 0;
        std::size_t encryption_weakness = 0;
    };

private:
    std::unordered_set<std::size_t> m_preamble;
    std::list<std::size_t>          m_order;

    std::size_t   m_preamble_length = 0;

    xmas_sequence m_data;

public:
    fault_detector(const xmas_sequence & p_data, const std::size_t p_preamble_length = 25) : 
        m_data(p_data),
        m_preamble_length(p_preamble_length)
    {
        for (std::size_t i = 0; i < m_preamble_length; i++) {
            m_preamble.insert(m_data[i]);
            m_order.push_back(m_data[i]);
        }
    }

public:
    report analyse() {
        report result;

        result.invalid_value = find_invalid_value();
        result.encryption_weakness = find_encryption_weakness(result.invalid_value);

        return result;
    }

private:
    std::size_t find_encryption_weakness(const std::size_t p_value) {
        auto lborder = 0;
        auto rborder = 1;

        std::size_t current_sum = m_data[lborder] + m_data[rborder];

        while (current_sum != p_value) {
            if (current_sum < p_value) {
                current_sum += m_data[++rborder];
            }
            else if (current_sum > p_value) {
                current_sum -= m_data[lborder++];
            }
        }

        if (current_sum != p_value) {
            throw std::invalid_argument("Encryption weakness is not found.");
        }

        const auto minmax = std::minmax_element(m_data.begin() + lborder, m_data.begin() + rborder);

        return *minmax.first + *minmax.second;
    }

    std::size_t find_invalid_value() {
        for (std::size_t i = m_preamble_length; i < m_data.size(); i++) {
            const std::size_t current_value = m_data[i];

            if (!check_sum(current_value)) {
                return current_value;
            }

            m_preamble.erase(m_order.front());
            m_order.pop_front();

            m_preamble.insert(current_value);
            m_order.push_back(current_value);
        }

        return 0;
    }

    bool check_sum(const std::size_t p_value) {
        for (std::size_t candidate : m_preamble) {
            const std::size_t delta = p_value - candidate;
            if (m_preamble.find(delta) != m_preamble.end()) {
                return true;
            }
        }

        return false;
    }
};


int main() {
    const xmas_sequence data = read_data();
    const fault_detector::report result = fault_detector(data, 25).analyse();

    std::cout << "Invalid value in the XMAS sequence: " << result.invalid_value << std::endl;
    std::cout << "Encryption weakness in the XMAS sequence: " << result.encryption_weakness << std::endl;

    return 0;
}
