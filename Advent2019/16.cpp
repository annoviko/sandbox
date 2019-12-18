#include <fstream>
#include <iostream>
#include <vector>
#include <string>


std::vector<long> read_signal() {
    std::vector<long> result;

    std::ifstream stream("input.txt");

    std::string line;
    std::getline(stream, line);

    stream.close();

    for (const auto value : line) {
        result.push_back(std::stol(std::string(1, value)));
    }

    return result;
}


class pattern_builder {
private:
    const std::vector<long> basic_pattern = { 0, 1, 0, -1 };
    std::size_t m_repeat = 1;

public:
    pattern_builder() { }

public:
    void next() { m_repeat++; }

    long get_value(std::size_t p_index_value) {
        const std::size_t index = ((p_index_value + 1) / m_repeat) % basic_pattern.size();
        return basic_pattern[index];
    }
};


class fft {
private:
    std::vector<long> m_signal;
    bool              m_repeat = false;

public:
    fft(const std::vector<long>& p_signal, const bool p_repeat = false) :
        m_signal(p_signal),
        m_repeat(p_repeat)
    { }

public:
    std::vector<long> process(const std::size_t p_steps, const std::size_t p_offset = 0) {
        for (std::size_t i = 0; i < p_steps; i++) {
            std::cout << "Step " << i << std::endl;
            calculate_output(p_offset);
        }

        return m_signal;
    }

private:
    void calculate_output(const std::size_t p_offset) {
        pattern_builder builder;
        for (std::size_t i = p_offset; i < m_signal.size(); i++) {
            long value = calculate_value(builder, i, m_signal);
            m_signal[i] = value;
            builder.next();
        }
    }

    long calculate_value(pattern_builder & builder, const std::size_t p_offset, const std::vector<long>& p_signal) {
        long result = 0;

        for (std::size_t i = p_offset; i < p_signal.size(); i++) {
            long pattern_value = builder.get_value(i);
            long value = pattern_value * p_signal[i];
            result += value;
        }

        return abs(result) % 10;
    }
};


std::ostream& operator << (std::ostream& p_stream, const std::vector<long>& p_vec) {
    p_stream << "[ ";
    for (const auto& item : p_vec) {
        p_stream << item << " ";
    }
    p_stream << "]";
    return p_stream;
}


int main() {
    auto signal = read_signal();
    auto result = fft(signal).process(100);

    std::cout << "The first 8 digits: ";
    for (std::size_t i = 0; i < 8; i++) {
        std::cout << result.at(i);
    }
    std::cout << std::endl << std::endl;


#if 1
    long offset = 0;
    for (std::size_t i = 0; i < 7; i++) {
        offset *= 10;
        offset += signal[i];
    }

    std::cout << "Offset for the signal: " << offset << std::endl;

    std::cout << "Prepare real signal." << std::endl;
    std::vector<long> real_signal;
    real_signal.reserve(signal.size());

    for (std::size_t repeat = 0; repeat < 10000; repeat++) {
        for (auto& value : signal) {
            real_signal.push_back(value);
        }
    }

    std::cout << "Signal size: " << real_signal.size() << ", Length to analyse: " << real_signal.size() - offset << std::endl;

    std::cout << "Run encoding." << std::endl;
    result = fft(real_signal).process(100, offset);

    std::cout << "The eight-digit message embedded in the final output list: ";
    for (std::size_t i = offset; i < static_cast<std::size_t>(offset) + 8; i++) {
        std::cout << result.at(i);
    }
    std::cout << std::endl << std::endl;
#endif

    return 0;
}