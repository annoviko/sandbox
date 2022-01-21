#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>


using diagnostic_report = std::set<std::string>;


diagnostic_report read_input() {
    std::ifstream stream("input.txt");

    diagnostic_report result;
    for (std::string line; std::getline(stream, line); ) {
        result.insert(line);
    }

    return result;
}


class analyser {
protected:
    using bit_statistics = std::vector<std::size_t>;

protected:
    diagnostic_report m_report;
    bit_statistics m_bit_stat;

    std::size_t m_consuption_power = 0;
    std::size_t m_life_support_rating = 0;

public:
    analyser(const diagnostic_report& p_report) : m_report(p_report) {
        if (m_report.empty()) {
            throw std::exception("empty diagnostic report");
        }

        m_bit_stat = bit_statistics(get_value_size(), 0);
    }

public:
    analyser & analyse() {
        for (const auto & value : m_report) {
            analyse_value(value);
        }

        calculate_consumption_power();
        calculate_life_support_rating();

        return *this;
    }

    std::size_t get_consumption_power() {
        return m_consuption_power;
    }

    std::size_t get_life_support_rating() {
        return m_life_support_rating;
    }

protected:
    void analyse_value(const std::string & p_value) {
        for (std::size_t i = 0; i < p_value.size(); i++) {
            if (p_value[i] == '1') {
                m_bit_stat[i]++;
            }
        }
    }


    std::size_t get_value_size() const {
        return m_report.begin()->size();
    }


    bool is_one_common(const bit_statistics & p_stats, const std::size_t p_report_length, const std::size_t p_index) {
        const std::size_t amount_zeros = p_report_length - p_stats[p_index];
        return p_stats[p_index] >= amount_zeros;
    }


    bool is_zero_common(const bit_statistics & p_stats, const std::size_t p_report_length, const std::size_t p_index) {
        const std::size_t amount_zeros = p_report_length - p_stats[p_index];
        return p_stats[p_index] <= amount_zeros;
    }


    void calculate_consumption_power() {
        const std::size_t gamma_rate = calculate_gamma_rate();
        const std::size_t epsilon_rate = calculate_epsilon_rate();

        m_consuption_power = gamma_rate * epsilon_rate;
    }


    void calculate_life_support_rating() {
        const std::size_t oxygen_rating = calculate_oxygen_generator_rating();
        const std::size_t co2_rating = calculate_co2_scrubber_rating();

        m_life_support_rating = oxygen_rating * co2_rating;
    }


    std::size_t calculate_gamma_rate() {
        std::size_t gamma_rate = 0;
        const std::size_t length = get_value_size();
        for (std::size_t i = 0; i < length; i++) {
            gamma_rate <<= 1;
            gamma_rate += is_one_common(m_bit_stat, m_report.size(), i) ? 1 : 0;
        }

        return gamma_rate;
    }

    std::size_t calculate_epsilon_rate() {
        std::size_t epsilon_rate = 0;
        const std::size_t length = get_value_size();
        for (std::size_t i = 0; i < length; i++) {
            epsilon_rate <<= 1;
            epsilon_rate += is_one_common(m_bit_stat, m_report.size(), i) ? 0 : 1;
        }

        return epsilon_rate;
    }


    std::size_t calculate_oxygen_generator_rating() {
        return calculate_gas_rating(true);
    }


    std::size_t calculate_co2_scrubber_rating() {
        return calculate_gas_rating(false);
    }


    std::size_t calculate_gas_rating(bool p_is_oxygen) {
        diagnostic_report local_report = m_report;
        bit_statistics stats = m_bit_stat;

        const std::size_t length = get_value_size();
        for (std::size_t i = 0; i < length; i++) {
            bool bit_priority = is_one_common(stats, local_report.size(), i);
            if (!p_is_oxygen) {
                bit_priority = !bit_priority;
            }

            filter(i, bit_priority, local_report, stats);

            if (local_report.size() == 1) {
                return std::stoul(*local_report.begin(), nullptr, 2);
            }
        }

        throw std::exception("impossible to identify gas rating");
    }


    void filter(const std::size_t p_index, const bool p_bit, diagnostic_report & p_values, bit_statistics & p_stats) {
        std::set<std::string> result = { };
        const char bit = p_bit ? '1' : '0';
        for (const auto & value : p_values) {
            if (value[p_index] == bit) {
                result.insert(value);
            }
        }

        bit_statistics stats = bit_statistics(get_value_size(), 0);
        for (const auto & value : result) {
            for (std::size_t i = p_index + 1; i < value.size(); i++) {
                if (value[i] == '1') {
                    stats[i]++;
                }
            }
        }

        p_stats = std::move(stats);
        p_values = std::move(result);
    }
};


int main() {
    const auto diagnostic_report = read_input();

    analyser controller = analyser(diagnostic_report).analyse();

    std::cout << "Submarine consumption power: " << controller.get_consumption_power() << std::endl;
    std::cout << "Submarine life rating: " << controller.get_life_support_rating() << std::endl;

    return 0;
}
