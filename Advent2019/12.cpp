#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include <boost/math/common_factor_rt.hpp>

struct position {
public:
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;

public:
    position& operator+=(const position& p_other) {
        x += p_other.x;
        y += p_other.y;
        z += p_other.z;
        return *this;
    }

    friend bool operator==(const position& p1, const position& p2) {
        return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z);
    }
};


std::ostream& operator << (std::ostream& p_stream, const position& p_coordinates) {
    p_stream << "<x=" << p_coordinates.x << ", y=" << p_coordinates.y << ", z=" << p_coordinates.z << ">";
    return p_stream;
}


position read_position(std::ifstream& p_stream) {
    position coordinates;

    std::string raw_line;
    std::getline(p_stream, raw_line);
    if (raw_line.empty()) {
        throw std::exception("Error: Nothing is read from the stream.");
    }

    std::regex pattern("<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>");
    std::smatch match;

    if (std::regex_search(raw_line, match, pattern) && match.size() > 1) {
        coordinates.x = std::stol(match.str(1));
        coordinates.y = std::stol(match.str(2));
        coordinates.z = std::stol(match.str(3));
    }
    else {
        throw std::exception("Error: Pattern mismatch - coordinates are not extracted.");
    }

    return coordinates;
}


std::vector<position> read_positions() {
    std::vector<position> result;

    std::ifstream infile("input.txt");
    for (std::size_t i = 0; i < 4; i++) {
        result.push_back(read_position(infile));
    }

    infile.close();
    return result;
}


class satellite {
public:
    position m_coordinates;
    position m_velocity;

public:
    satellite(const position & p_coordinates) : 
        m_coordinates(p_coordinates) 
    { }

public:
    void apply_gravity(satellite& p_other) {
        apply_coordinate_gravity(m_coordinates.x, m_velocity.x, p_other.m_coordinates.x, p_other.m_velocity.x);
        apply_coordinate_gravity(m_coordinates.y, m_velocity.y, p_other.m_coordinates.y, p_other.m_velocity.y);
        apply_coordinate_gravity(m_coordinates.z, m_velocity.z, p_other.m_coordinates.z, p_other.m_velocity.z);
    }

    int64_t get_total_energy() const {
        return get_potential_energy() * get_kinetic_energy();
    }

    int64_t get_potential_energy() const {
        return std::abs(m_coordinates.x) + std::abs(m_coordinates.y) + std::abs(m_coordinates.z);
    }

    int64_t get_kinetic_energy() const {
        return std::abs(m_velocity.x) + std::abs(m_velocity.y) + std::abs(m_velocity.z);
    }

    void simulate() {
        m_coordinates += m_velocity;
    }

public:
    friend bool operator==(const satellite& s1, const satellite& s2) {
        return (s1.m_coordinates == s2.m_coordinates) && (s1.m_velocity == s2.m_velocity);
    }

private:
    void apply_coordinate_gravity(const int64_t p1, int64_t& v1, const int64_t p2, int64_t& v2) {
        if (p1 > p2) {
            v1--;
            v2++;
        }
        else if (p1 < p2) {
            v1++;
            v2--;
        }
    }
};


class repeat_detector {
private:
    std::vector<position> m_initial;

    std::size_t m_counter = 0;
    position m_period = { -1, -1, -1 };
public:
    repeat_detector(const std::vector<satellite>& p_initial) {
        for (const auto& moon : p_initial) {
            m_initial.push_back(moon.m_coordinates);
        }
    }

public:
    bool detect(const std::vector<satellite>& p_state) {
        m_counter++;

        bool found = true;
        found &= check_x(p_state);
        found &= check_y(p_state);
        found &= check_z(p_state);

        return found;
    }

    position get_period() const {
        return m_period;
    }

private:
    bool check_x(const std::vector<satellite>& p_state) {
        if (m_period.x != -1) { return true; }

        for (std::size_t i = 0; i < p_state.size(); i++) {
            if ((m_initial[i].x != p_state[i].m_coordinates.x) || (p_state[i].m_velocity.x != 0)) {
                return false;
            }
        }

        m_period.x = m_counter;
        return true;
    }

    bool check_y(const std::vector<satellite>& p_state) {
        if (m_period.y != -1) { return true; }

        for (std::size_t i = 0; i < p_state.size(); i++) {
            if ((m_initial[i].y != p_state[i].m_coordinates.y) || (p_state[i].m_velocity.y != 0)) {
                return false;
            }
        }

        m_period.y = m_counter;
        return true;
    }

    bool check_z(const std::vector<satellite>& p_state) {
        if (m_period.z != -1) { return true; }

        for (std::size_t i = 0; i < p_state.size(); i++) {
            if ((m_initial[i].z != p_state[i].m_coordinates.z) || (p_state[i].m_velocity.z != 0)) {
                return false;
            }
        }

        m_period.z = m_counter;
        return true;
    }
};


class jupiter_system {
private:
    std::vector<satellite> m_moons;

public:
    jupiter_system() {
        auto moon_positions = read_positions();
        for (const auto& pos : moon_positions) {
            m_moons.emplace_back(pos);
        }
    }

public:
    std::size_t simulate(const std::size_t p_steps) {
        for (std::size_t i = 0; i < p_steps; i++) {
            apply_gravity();
            make_step();
        }

        return get_total_energy();
    }

    std::size_t simulate_until_initial_state() {
        repeat_detector detector(m_moons);
        do {
            apply_gravity();
            make_step();
        } while (!detector.detect(m_moons));

        auto period = detector.get_period();
        std::size_t result = boost::math::lcm<int64_t>(boost::math::lcm<int64_t>(period.x, period.y), period.z);
        return result;
    }

private:
    void apply_gravity() {
        for (std::size_t i = 0; i < m_moons.size(); i++) {
            for (std::size_t j = i + 1; j < m_moons.size(); j++) {
                m_moons[i].apply_gravity(m_moons[j]);
            }
        }
    }

    void make_step() {
        for (auto& moon : m_moons) {
            moon.simulate();
        }
    }

    int64_t get_total_energy() {
        int64_t result = 0;
        for (auto& moon : m_moons) {
            result += moon.get_total_energy();
        }

        return result;
    }
};



int main() {
    std::cout << "The total energy in the system: " << jupiter_system().simulate(1000) << std::endl;
    std::cout << "Amount steps before the system returns to initial state: " << jupiter_system().simulate_until_initial_state() << std::endl;

    return 0;
}
