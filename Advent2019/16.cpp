#include <algorithm>
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


struct segment {
    long left_border = -1;
    long right_border = -1;
    long length = -1;
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
    std::vector<long> process(const std::size_t p_steps) {
        for (std::size_t i = 0; i < p_steps; i++) {
            std::cout << "Step " << i << std::endl;
            calculate_output();
        }

        return m_signal;
    }

private:
    void calculate_output() {
        std::vector<long> result(m_signal.size(), 0);

        long middle = std::floor(m_signal.size() / 2);

        std::vector<long> cm(m_signal.size(), 0);
        std::vector<segment> borders(m_signal.size());

        for (long i = m_signal.size() - 1; i >= middle; i--) {
            cm[0] = cm[0] + (m_signal[i] % 10);
            result[i] = std::abs(cm[0] % 10);
        }

        borders[0].left_border = middle;
        borders[0].right_border = m_signal.size() - 1;

        for (long i = middle - 1; i >= 0; i--) {
            long sign = 1;
            long repeat = i + 1;

            for (long index_cm = 0; index_cm < cm.size(); index_cm += 2, sign *= -1) {
                long new_left_border = i + repeat * index_cm;
                long new_right_border = std::min(new_left_border + repeat - 1, (long)m_signal.size() - 1);

                if (new_left_border >= m_signal.size()) {
                    break;
                }

                long amount_to_add = std::min(std::min(repeat, index_cm + 1), new_right_border - new_left_border + 1);

                for (long index_value = new_left_border; index_value < new_left_border + amount_to_add; index_value++) {
                    cm[index_cm] += sign * (m_signal[index_value] % 10);
                }

                long prev_left_border = borders[index_cm].left_border;
                long prev_right_border = borders[index_cm].right_border;

                long remove_border = std::max(new_right_border + 1, prev_left_border);

                if (prev_left_border != -1) {
                    for (long index_to_remove = remove_border; index_to_remove <= prev_right_border; index_to_remove++) {
                        cm[index_cm] -= sign * (m_signal[index_to_remove] % 10);
                    }
                }

                borders[index_cm].left_border = new_left_border;
                borders[index_cm].right_border = new_right_border;

                result[i] += cm[index_cm];
            }
            
            result[i] = std::abs(result[i] % 10);
        }

        m_signal = std::move(result);
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



    std::size_t offset = 0;
    for (std::size_t i = 0; i < 7; i++) {
        offset *= 10;
        offset += signal[i];
    }

    std::cout << "Offset for the signal: " << offset << std::endl;

    std::cout << "Prepare real signal." << std::endl;
    std::vector<long> real_signal;
    real_signal.reserve(signal.size() * 10000);

    for (std::size_t repeat = 0; repeat < 10000; repeat++) {
        for (auto& value : signal) {
            real_signal.push_back(value);
        }
    }

    std::cout << "Prepare message." << std::endl;
    std::vector<long> message;
    message.reserve(real_signal.size() - offset);
    for (std::size_t i = offset; i < real_signal.size(); i++) {
        message.push_back(real_signal[i]);
    }

    std::cout << "Run encoding." << std::endl;
    result = fft(real_signal).process(100);

    std::cout << "The eight-digit message embedded in the final output list: ";
    for (std::size_t i = offset; i < (offset + 8); i++) {
        std::cout << result.at(i);
    }
    std::cout << std::endl << std::endl;


    return 0;
}