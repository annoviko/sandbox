#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


struct record {
    std::uint64_t time;
    std::uint64_t distance;
};


class record_analyser {
private:
    record m_entry;

public:
    record_analyser(const record& p_record) : m_entry(p_record) { }


public:
    std::uint64_t ways_to_win() {
        std::uint64_t ways_to_win = 0;

        for (int i = 1; i < m_entry.time; i++) {
            const std::uint64_t velocity = i;
            const std::uint64_t distance = (m_entry.time - i) * velocity;

            if (distance > m_entry.distance) {
                ways_to_win++;
            }
        }

        return ways_to_win;
    }


    std::uint64_t ways_to_win_binary_search() {
        std::uint64_t ways_to_win = 0;

        /* find left border */
        std::uint64_t lborder = 1;
        std::uint64_t rborder = m_entry.time / 2;   /* the best possible result */

        while (rborder - lborder > 1) {
            std::uint64_t middle = (lborder + rborder) / 2;
            std::uint64_t distance = simulate(middle);

            if (distance > m_entry.distance) {
                rborder = middle;   /* rborder should be always inside (higher than the record) */
            }
            else {
                lborder = middle;
            }
        }

        std::uint64_t left = rborder;  /* point to value inside (the first value greater than the record) */

        /* find right border */
        lborder = m_entry.time / 2;
        rborder = m_entry.time - 1;

        while (rborder - lborder > 1) {
            std::uint64_t middle = (lborder + rborder) / 2;
            std::uint64_t distance = simulate(middle);

            if (distance > m_entry.distance) {
                lborder = middle;   /* left border should be always inside (higher than the record) */
            }
            else {
                rborder = middle;
            }
        }

        std::uint64_t right = lborder;   /* point to value inside (the last value greater than the record) */
        return right - left + 1;    /* both inside, so add +1 as zero shift (for example, 5, 6, 7 - the amount is 3, not 2) */
    }

private:
    std::uint64_t simulate(std::uint64_t t) {
        const std::uint64_t velocity = t;
        return (m_entry.time - t) * velocity;
    }
};


std::vector<record> read_input() {
    std::fstream f("input.txt");

    std::string line;
    std::getline(f, line);

    std::vector<record> result;

    std::stringstream stream(line);
    stream >> line;
    while (stream.good()) {
        std::uint64_t t = 0;
        stream >> t;

        result.push_back({ t, 0 });
    }

    std::getline(f, line);
    stream = std::stringstream(line);
    stream >> line;

    for (int i = 0; i < result.size(); i++) {
        stream >> result[i].distance;
    }

    return result;
}


class adviser {
private:
    std::vector<record> m_records;

public:
    adviser(const std::vector<record>& p_records) : m_records(p_records) { }

public:
    std::uint64_t ways_to_win() {
        int ways_to_win = 0;

        for (const auto& r : m_records) {
            const std::uint64_t ways = record_analyser(r).ways_to_win_binary_search();
            if (ways > 0) {
                if (ways_to_win == 0) {
                    ways_to_win = 1;
                }

                ways_to_win *= ways;
            }
        }

        return ways_to_win;
    }
};


record correct_input(const std::vector<record>& records) {
    std::string time;
    std::string distance;

    for (auto& r : records) {
        time += std::to_string(r.time);
        distance += std::to_string(r.distance);
    }

    return { std::stoull(time), std::stoull(distance) };
}


int main() {
    auto input = read_input();

    std::cout << "Multiplication of number of ways you can beat the record: " << adviser(input).ways_to_win() << std::endl;
    std::cout << "Number of ways you can beat the record: " << record_analyser(correct_input(input)).ways_to_win_binary_search() << std::endl;

    return 0;
}