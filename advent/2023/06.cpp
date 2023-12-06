#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


struct record {
    std::uint64_t time;
    std::uint64_t distance;
};


enum class algorithm {
    BRUTE_FORCE = 0,        /* O(N)         */
    BINARY_SEARCH = 1,      /* O(log(N))    */
    MATH = 2                /* O(1)         */
};


class record_analyser {
private:
    record m_entry;

public:
    record_analyser(const record& p_record) : m_entry(p_record) { }


public:
    std::uint64_t ways_to_win(const algorithm& method) {
        switch (method) {
        case algorithm::BRUTE_FORCE:
            return ways_to_win_brute_force();

        case algorithm::BINARY_SEARCH:
            return ways_to_win_binary_search();

        case algorithm::MATH:
            return ways_to_win_math();
        }
    }


    std::uint64_t ways_to_win_brute_force() {   /* O(N) */
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


    std::uint64_t ways_to_win_binary_search() { /* O(log(N)) */
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


    std::uint64_t ways_to_win_math() {  /* O(1) */
        /* 
		
		s[n] = v[n] * (t - n) = n * (t - n)
		
		we know `s` and `t` for the record, so we can build an equation:
		
		s = x * (t - x)
		
		x^2 - t * x + s = 0
		
		We need to know length between x1 and x2.
		
		*/

        auto D = [](double t, double s) -> double {
            return std::pow(t, 2) - 4.0 * s;
        };

        auto X1 = [&D](double t, double s) {
            return (t + std::sqrt(D(t, s))) / 2.0;
        };

        auto X2 = [&D](double t, double s) {
            return (t - std::sqrt(D(t, s))) / 2.0;
        };

        std::int64_t x1 = std::ceil(X1(m_entry.time, m_entry.distance) - 1);    /* right border - correct to be inside */
        std::int64_t x2 = std::floor(X2(m_entry.time, m_entry.distance) + 1);   /* left border - correct to be inside  */

        std::int64_t length = x1 - x2 + 1;  /* both inside, so add +1 as zero shift (for example, 5, 6, 7 - the amount is 3, not 2) */
        return length;
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
    std::uint64_t ways_to_win(const algorithm& method) {
        int ways_to_win = 0;

        for (const auto& r : m_records) {
            const std::uint64_t ways = record_analyser(r).ways_to_win(method);
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

    std::cout << "Multiplication of number of ways you can beat the record: " << adviser(input).ways_to_win(algorithm::MATH) << std::endl;
    std::cout << "Number of ways you can beat the record: " << record_analyser(correct_input(input)).ways_to_win(algorithm::MATH) << std::endl;

    return 0;
}