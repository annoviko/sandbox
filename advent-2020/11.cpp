#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


using seats_map = std::vector<std::string>;


seats_map read_seats(const std::string & p_file) {
    std::ifstream stream(p_file);

    seats_map result;

    for (std::string line; std::getline(stream, line);) {
        result.push_back(std::move(line));
    }

    return result;
}


class simulator {
public:
    enum class proc_t {
        incorrect,
        correct
    };

private:
    seats_map m_seats;
    long long m_height = 0;
    long long m_width = 0;

    std::function<bool(long long, long long, long long, long long)> is_free_checker;
    long long m_threshold = 0;

public:
    simulator(const seats_map & p_seats) :
        m_seats(p_seats),
        m_height(m_seats.size()),
        m_width(m_seats[0].size())
    { }

public:
    std::size_t simulate(const simulator::proc_t p_type) {
        if (p_type == simulator::proc_t::incorrect) {
            is_free_checker = [this](long long i, long long j, long long di, long long dj) { return is_free_place(i, j, di, dj); };
            m_threshold = 4;
        }
        else {
            is_free_checker = [this](long long i, long long j, long long di, long long dj) { return is_free_line(i, j, di, dj); };
            m_threshold = 5;
        }

        while (iterate()) { }
        return get_amount_occupied_places();
    }

private:
    std::size_t get_amount_occupied_places() {
        std::size_t result = 0;

        for (auto & row : m_seats) {
            for (auto & value : row) {
                if (value == '#') {
                    result++;
                }
            }
        }

        return result;
    }

    bool iterate() {
        seats_map next_state(m_height, std::string(m_width, ' '));
        bool changes_detected = false;

        for (long long i = 0; i < m_height; i++) {
            for (long long j = 0; j < m_width; j++) {
                evaluate_place(i, j, next_state);
                changes_detected |= (next_state[i][j] != m_seats[i][j]);
            }
        }

        m_seats = std::move(next_state);
        return changes_detected;
    }

    void evaluate_place(const long long i, const long long j, seats_map & p_state) {
        const char status = m_seats[i][j];

        switch (status) {
        case '.':
            p_state[i][j] = m_seats[i][j];
            break;
        case 'L':
            p_state[i][j] = get_next_state_for_free_place(i, j);
            break;
        case '#':
            p_state[i][j] = get_next_state_for_occupied_place(i, j);
            break;
        default:
            throw std::invalid_argument("Invalid marker on the map.");
        }
    }

    bool is_valid_position(const long long i, const long long j) {
        if ((i >= 0 && i < m_height) && (j >= 0 && j < m_width)) {
            return true;
        }

        return false;
    }

    bool is_free_line(const long long i, const long long j, const long long di, const long long dj) {
        auto in = i + di, jn = j + dj;

        while (is_valid_position(in, jn)) {
            if (m_seats[in][jn] == '#') {
                return false;
            }
            else if (m_seats[in][jn] == 'L') {
                return true;
            }

            in += di;
            jn += dj;
        }

        return true;
    }

    bool is_free_place(const long long i, const long long j, const long long di, const long long dj) {
        const long long in = i + di;
        const long long jn = j + dj;

        if (!is_valid_position(in, jn)) {
            return true;
        }

        return m_seats[in][jn] != '#';
    }

    char get_next_state_for_free_place(const long long i, const long long j) {
        const std::vector<std::pair<long long, long long>> differences = {
            { 0, 1 }, { 0, -1 }, { 1, 0 }, { 1, 1 }, { 1, -1 }, { -1, 0 }, { -1, 1 }, { -1, -1 }
        };

        for (const auto & delta : differences) {
            if (!is_free_checker(i, j, delta.first, delta.second)) {
                return 'L';
            }
        }

        return '#';
    }

    char get_next_state_for_occupied_place(const long long i, const long long j) {
        const std::vector<std::pair<long long, long long>> differences = {
            { 0, 1 }, { 0, -1 }, { 1, 0 }, { 1, 1 }, { 1, -1 }, { -1, 0 }, { -1, 1 }, { -1, -1 }
        };

        std::size_t occupied_neighbors = 0;
        for (const auto & delta : differences) {
            if (!is_free_checker(i, j, delta.first, delta.second)) {
                occupied_neighbors++;
            }

            if (occupied_neighbors >= m_threshold) {
                return 'L';
            }
        }

        return '#';
    }
};



int main() {
    const auto seats = read_seats("input.txt");

    std::cout << "The amount of occupied places (suggestion): " << simulator(seats).simulate(simulator::proc_t::incorrect) << std::endl;
    std::cout << "The amount of occupied places (actual): " << simulator(seats).simulate(simulator::proc_t::correct) << std::endl;

    return 0;
}
