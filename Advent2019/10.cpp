#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>
#include <vector>
#include <set>
#include <tuple>


using space_map = std::vector<std::string>;


static const double BASE_SPECIFIC_VALUE = std::numeric_limits<double>::max();

static const double POSITIVE_NAN = 10000;
static const double NEGATIVE_NAN = 11000;

static const double POSITIVE_ZERO = 12000;
static const double NEGATIVE_ZERO = 13000;


struct position {
public:
    double x = -1;
    double y = -1;

public:
    bool operator==(const position& p_other) const {
        return x == p_other.x && y == p_other.y;
    }

    bool operator!=(const position& p_other) const {
        return !(*this == p_other);
    }
};


std::ostream& operator<<(std::ostream& p_stream, const position & p_instance) {
    p_stream << "(" << p_instance.x << ", " << p_instance.y << ")";
    return p_stream;
}


class asteroid_detector {
private:
    std::list<position> m_asteroids;
    std::size_t m_tolerance;

public:
    asteroid_detector(const std::size_t m_precision = 5) : 
        m_asteroids(extract_asteroids(read_space_map())),
        m_tolerance(1)
    {
        for (std::size_t i = 0; i < m_precision; i++) {
            m_tolerance *= 10;
        }
    }

public:
    std::tuple<position, std::size_t> find_optimal_detection_position() const {
        std::size_t maximum_detection = 0;
        position optimal_position;

        for (const auto& position : m_asteroids) {
            std::size_t detected_asteroids = detect_asteroids(position);
            if (detected_asteroids > maximum_detection) {
                maximum_detection = detected_asteroids;
                optimal_position = position;
            }
        }

        return std::make_tuple(optimal_position, maximum_detection);
    }

private:
    std::size_t detect_asteroids(const position& detector_position) const {
        std::set<double> angles_0_to_pi;
        std::set<double> angles_pi_to_2p;

        for (const auto& asteroid_position : m_asteroids) {
            if (asteroid_position == detector_position) { continue; }

            position asteroid_adjacent_vector = { asteroid_position.x - detector_position.x, asteroid_position.y - detector_position.y };
            auto angle = calculate_angle({ 1, 0 }, asteroid_adjacent_vector);

            if (asteroid_adjacent_vector.y >= 0) {
                angles_0_to_pi.insert(angle);
            }
            else {
                angles_pi_to_2p.insert(angle);
            }
        }

        // std::cout << detector_position << ": " << angles_0_to_pi.size() + angles_pi_to_2p.size() << std::endl;

        return angles_0_to_pi.size() + angles_pi_to_2p.size();
    }

    double calculate_angle(const position & p1, const position & p2) const {
        double ab = p1.x * p2.x + p1.y * p2.y;
        double a = std::sqrt(std::pow(p1.x, 2) + std::pow(p1.y, 2));
        double b = std::sqrt(std::pow(p2.x, 2) + std::pow(p2.y, 2));

        double theta = ab / (a * b);
        return std::round(theta * m_tolerance);
    }

    std::list<position> extract_asteroids(const space_map & p_map) {
        std::list<position> result;

        for (std::size_t row = 0; row < p_map.size(); row++) {
            for (std::size_t col = 0; col < p_map[0].size(); col++) {
                if (p_map[row][col] == '#') {
                    result.push_back({ static_cast<double>(col), static_cast<double>(row) });
                }
            }
        }

        return result;
    }

    space_map read_space_map() {
        space_map map;

        std::ifstream infile("input.txt");

        std::string row_map;
        while (std::getline(infile, row_map)) {
            map.push_back(std::move(row_map));
        }
        infile.close();

        if (map.empty()) {
            throw std::exception("Error: Map is not loaded.");
        }

        return map;
    }
};


int main() {
    asteroid_detector detector;
    auto result = detector.find_optimal_detection_position();

    std::cout << "Optimal position: " << std::get<0>(result) << std::endl;
    std::cout << "Maximum amount of asteroids that can be detected: " << std::get<1>(result) << std::endl;

    return 0;
}
