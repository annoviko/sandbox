#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <map>
#include <set>
#include <string>
#include <vector>


using space_map = std::vector<std::string>;


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

std::ostream& operator<<(std::ostream& p_stream, const space_map& p_instance) {
    for (const auto& row : p_instance) {
        p_stream << row << std::endl;
    }

    return p_stream;
}


struct detection_result {
    position m_position;
    std::map<double, position> m_asteroids;
};


class asteroid_detector {
private:
    std::list<position> m_asteroids;
    std::size_t m_tolerance;

public:
    asteroid_detector(const space_map & p_map, const std::size_t m_precision = 5) : 
        m_asteroids(extract_asteroids(p_map)),
        m_tolerance(1)
    {
        for (std::size_t i = 0; i < m_precision; i++) {
            m_tolerance *= 10;
        }
    }

public:
    detection_result find_optimal_detection_position() const {
        std::size_t maximum_detection = 0;
        detection_result result;

        for (const auto& position : m_asteroids) {
            detection_result candidate = detect_asteroids(position);
            if (candidate.m_asteroids.size() > maximum_detection) {
                maximum_detection = candidate.m_asteroids.size();
                result = std::move(candidate);
            }
        }

        return result;
    }

    detection_result detect_asteroids(const position& p_position, const space_map& p_map) {
        m_asteroids = extract_asteroids(p_map);
        return detect_asteroids(p_position);
    }

private:
    detection_result detect_asteroids(const position& detector_position) const {
        detection_result result;
        std::map<double, double> distances;

        result.m_position = detector_position;

        for (const auto& asteroid_position : m_asteroids) {
            if (asteroid_position == detector_position) { continue; }

            position asteroid_adjacent_vector = { asteroid_position.x - detector_position.x, asteroid_position.y - detector_position.y };
            auto angle = calculate_angle({ 0, 1 }, asteroid_adjacent_vector);
            auto distance = euclidean_distance(detector_position, asteroid_position);

            if (asteroid_adjacent_vector.x < 0) {
                if (angle > 0) {
                    /* area from pi to 3pi/2 */
                    angle = (1.0 - angle) + 1.0;
                }
                else {
                    /* area from 3pi/2 to 2pi */
                    angle = -1 * angle + 2;
                }
            }

            if (result.m_asteroids.count(angle) > 0) {
                if (distance < distances[angle]) {
                    result.m_asteroids[angle] = asteroid_position;
                }
            }
            else {
                result.m_asteroids[angle] = asteroid_position;
                distances[angle] = distance;
            }
        }

        return result;
    }

    double euclidean_distance(const position& p1, const position& p2) const {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }

    double calculate_angle(const position & p1, const position & p2) const {
        double ab = p1.x * p2.x + p1.y * p2.y;
        double a = std::sqrt(std::pow(p1.x, 2) + std::pow(p1.y, 2));
        double b = std::sqrt(std::pow(p2.x, 2) + std::pow(p2.y, 2));

        double theta = ab / (a * b);
        return std::round(theta * m_tolerance) / m_tolerance;
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
};


int main() {
    space_map map = read_space_map();

    asteroid_detector detector(map);
    auto result = detector.find_optimal_detection_position();

    auto station_position = result.m_position;
    std::cout << "Optimal position: " << result.m_position << std::endl;
    std::cout << "Maximum amount of asteroids that can be detected: " << result.m_asteroids.size() << std::endl << std::endl;

    std::cout << "Deploy station..." << std::endl;
    std::cout << "Start vaporation..." << std::endl << std::endl;

    position asteroid;
    std::size_t count = 0;

    while (count < 200) {
        for (const auto& pair : result.m_asteroids) {
            count++;

            if (count == 200) {
                asteroid = pair.second;
                break;
            }
        }

        if (count < 200) {
            result = detector.detect_asteroids(station_position, map);
        }
    }

    std::cout << "200th asteroid: " << asteroid << std::endl;
    std::cout << "Answer: " << asteroid.x * 100 + asteroid.y << std::endl;

    return 0;
}
