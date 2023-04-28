#include <cmath>
#include <vector>
#include <string>


struct position {
public:
    double long x = 0;
    double long y = 0;
};


struct segment {
public:
    position p1;
    position p2;
};


class math_vector {
public:
    double long x = 0;
    double long y = 0;

public:
    math_vector(const position& p) :
        x(p.x), y(p.y)
    { }

    math_vector(const position& p1, const position& p2) {
        x = p2.x - p1.x;
        y = p2.y - p1.y;
    }

    long double dot(const math_vector& p_other) const {
        return x * p_other.x + y * p_other.y;
    }

    long double norm_square() const {
        return x * x + y * y;
    }

    math_vector projection(const math_vector& p_other) const {
        const auto scalar_projection = dot(p_other) / norm_square();
        math_vector vector_projection({ x * scalar_projection, y * scalar_projection });
        return vector_projection;
    }
};


double long euclidean_distance(const position& p1, const position& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


class solution {
public:
    std::vector<std::size_t> get_greatest_danger(const segment& p_hurricane, const std::vector<position> p_coords) {
        math_vector hurricane_vector(p_hurricane.p1, p_hurricane.p2);

        std::vector<std::size_t> result = { };
        long double smallest_distance = std::numeric_limits<long double>::max();

        for (std::size_t i = 0; i < p_coords.size(); i++) {
            const auto& coord = p_coords[i];

            const math_vector city_vector(p_hurricane.p1, coord);
            const math_vector projection_vector = hurricane_vector.projection(city_vector);

            position p = p_hurricane.p1;
            p.x += projection_vector.x;
            p.y += projection_vector.y;

            const long double distance = euclidean_distance(p, coord);

            if (std::fabs(smallest_distance - distance) < 0.0000001) {
                result.push_back(i);
            }
            else if (distance < smallest_distance) {
                smallest_distance = distance;
                result = { i };
            }
        }

        return result;
    }
};


#include <iostream>
#include <iomanip>

int main() {
    std::size_t n;
    std::cin >> n;

    for (std::size_t i = 0; i < n; i++) {
        double long x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        std::size_t m;
        std::cin >> m;

        std::vector<position> coords;
        std::vector<std::string> names;
        for (std::size_t j = 0; j < m; j++) {
            std::string name;
            long double x, y;
            std::cin >> name >> x >> y;

            coords.push_back({ x, y });
            names.push_back(name);
        }

        auto result = solution().get_greatest_danger({ x1, y1, x2, y2 }, coords);
        for (std::size_t j = 0; j < result.size(); j++) {
            std::cout << names[result[j]];
            if (j + 1 != result.size()) {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    return 0;
}
