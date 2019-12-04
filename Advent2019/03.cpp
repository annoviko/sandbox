#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <unordered_set>


struct coordinate {
    long x = 0;
    long y = 0;
    long length = 0;

    struct equal {
        bool operator()(const coordinate& point1, const coordinate& point2) const {
            return point1.x == point2.x && point1.y == point2.y;
        }
    };

    struct hash {
        std::size_t operator()(const coordinate & point) const {
            return std::hash<std::size_t>()(point.x + point.y * 1000);
        }
    };
};


using tracer = std::function<void(void)>;

using coordinate_set = std::unordered_set<coordinate, coordinate::hash, coordinate::equal>;


std::list<std::string> extract_wire_description(const std::string & line) {
    std::list<std::string> wire_actions;

    std::string action;
    std::stringstream stream(line);

    while (std::getline(stream, action, ',')) {
        wire_actions.push_back(action);
    }

    return wire_actions;
}


std::list<std::string> read_wire(std::ifstream& stream) {
    std::string wire_line;
    std::getline(stream, wire_line);
    return extract_wire_description(wire_line);
}


void read_wires(std::list<std::string> & wire1, std::list<std::string> & wire2) {
    std::ifstream infile("input.txt");

    wire1 = read_wire(infile);
    wire2 = read_wire(infile);

    infile.close();
}


void trace_wire(const long length, coordinate& position, coordinate_set& coordinates, tracer & method) {
    for (long step = 0; step < length; step++) {
        method();
        position.length++;
        coordinates.insert(position);
    }
}


coordinate_set get_wire_coordinates(const std::list<std::string> & wire) {
    coordinate_set coordinates;
    coordinate position;

    for (const auto& action : wire) {
        const char direction = action[0];
        const long steps = std::stol(action.substr(1, action.length() - 1));

        tracer method;

        switch (direction) {
        case 'R':
            method = [&position]() { position.x++; };
            break;
        case 'L':
            method = [&position]() { position.x--; };
            break;
        case 'U':
            method = [&position]() { position.y++; };
            break;
        case 'D':
            method = [&position]() { position.y--; };
            break;
        default:
            throw std::exception("Wrong type of direction is specified.");
        }
        
        trace_wire(steps, position, coordinates, method);
    }

    return coordinates;
}


std::list<coordinate> get_intersections(const coordinate_set& wire1, const coordinate_set& wire2) {
    std::list<coordinate> intersections;

    for (const auto& wire2_point : wire2) {
        auto wire1_point_iterator = wire1.find(wire2_point);
        if (wire1_point_iterator != wire1.cend()) {
            coordinate intersection_point = wire2_point;
            intersection_point.length = wire1_point_iterator->length + wire2_point.length;

            intersections.push_back(intersection_point);
        }
    }

    return intersections;
}


long manhattan_distance(const coordinate& point1, const coordinate& point2) {
    return abs(point1.x - point2.x) + abs(point1.y - point2.y);
}


long get_closest_distance(const std::list<coordinate>& intersections) {
    coordinate port_point;
    long closest_distance = std::numeric_limits<long>::max();

    for (const auto& position : intersections) {
        const long distance = manhattan_distance(port_point, position);
        if (distance < closest_distance) {
            closest_distance = distance;
        }
    }

    return closest_distance;
}


long get_minimum_energy(const std::list<coordinate>& intersections) {
    long minimum_energy = std::numeric_limits<long>::max();
    for (const auto& point : intersections) {
        if (minimum_energy > point.length) {
            minimum_energy = point.length;
        }
    }

    return minimum_energy;
}


int main() {
    std::list<std::string> wire1, wire2;
    read_wires(wire1, wire2);

    auto wire1_coordinates = get_wire_coordinates(wire1);
    auto wire2_coordinates = get_wire_coordinates(wire2);

    auto intersections = get_intersections(wire1_coordinates, wire2_coordinates);

    long closest_distance = get_closest_distance(intersections);
    long minimum_steps = get_minimum_energy(intersections);

    std::cout << "Closest distance to intersection between wires: " << closest_distance << std::endl;
    std::cout << "Minimum energy to intersection between wires: " << minimum_steps << std::endl;

    return 0;
}
