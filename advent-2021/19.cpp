#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>


struct point {
public:
    int x = 0;
    int y = 0;
    int z = 0;

public:
    point operator-(const point& p) const {
        return { x - p.x, y - p.y, z - p.z };
    }

    point operator+(const point& p) const {
        return { x + p.x, y + p.y, z + p.z };
    }

    bool operator<(const point& p) const {
        if (x != p.x) {
            return x < p.x;
        }

        if (y != p.y) {
            return y < p.y;
        }

        return z < p.z;
    }

    bool operator==(const point& p) const {
        return (x == p.x) && (y == p.y) && (z == p.z);
    }
};


std::ostream& operator<<(std::ostream& p_stream, const point& p_point) {
    p_stream << "(" << p_point.x << ", " << p_point.y << ", " << p_point.z << ")";
    return p_stream;
}


class rotater {
public:
    std::vector<std::set<point>> produce(const std::set<point>& p_vector) {
        std::vector<std::vector<point>> rotations; // row contains rotations for a single point

        for (const point& p : p_vector) {
            rotations.push_back(produce(p));
        }

        std::vector<std::set<point>> result;
        for (std::size_t index_rotation = 0; index_rotation < rotations[0].size(); index_rotation++) {
            std::set<point> points_with_the_same_rotation;
            for (std::size_t i = 0; i < rotations.size(); i++) {
                points_with_the_same_rotation.insert(rotations[i][index_rotation]);
            }

            result.push_back(points_with_the_same_rotation);
        }

        return result;
    }

    std::vector<point> produce(const point& p_point) {
        std::vector<point> result;

        auto p = p_point;
        for (std::size_t x_i = 0; x_i < 2; x_i++) {
            p.x *= -1;

            for (std::size_t y_i = 0; y_i < 2; y_i++) {
                p.y *= -1;

                for (std::size_t z_i = 0; z_i < 2; z_i++) {
                    p.z *= -1;

                    result.push_back(p);
                    result.push_back({ p.y, p.x, p.z });
                    result.push_back({ p.y, p.z, p.x });
                    result.push_back({ p.x, p.z, p.y });
                    result.push_back({ p.z, p.x, p.y });
                    result.push_back({ p.z, p.y, p.x });
                }
            }
        }

        return result;
    }
};


struct aligned_scanner {
    point position;
    std::set<point> beacons;
};


struct scanner_info {
    std::set<point> coordinates;
    std::vector<std::set<point>> rotations;
    std::set<int> not_matchable_with;

public:
    scanner_info(const std::set<point>& p_coord) : coordinates(p_coord) {
        rotations = rotater().produce(p_coord);
    }
};


class matcher {
private:
    std::vector<aligned_scanner> m_map;
    std::set<point> m_beacons;

public:
    void build_map(const std::vector<std::set<point>>& p_coordinates) {
        m_map.push_back({ { 0, 0, 0 }, p_coordinates[0] });
        m_beacons = p_coordinates[0];

        std::list<scanner_info> not_matched;
        for (std::size_t i = 1; i < p_coordinates.size(); i++) {
            not_matched.push_back(scanner_info(p_coordinates[i]));
        }

        while (!not_matched.empty()) {
            for (auto iter = not_matched.begin(); iter != not_matched.end(); iter++) {
                if (match_scanner(*iter)) {
                    not_matched.erase(iter);
                    break;
                }
            }
        }
    }


    std::size_t get_beacons_amount() {
        return m_beacons.size();
    }


private:
    bool match_scanner(scanner_info& p_info) {
        for (std::size_t j = 0; j < m_map.size(); j++) {
            if (p_info.not_matchable_with.find(j) != p_info.not_matchable_with.end()) {
                /* no need to match this scanner since is not matchable */
                continue;
            }

            if (check_overlap_in_rotations(m_map[j].beacons, p_info)) {
                return true;
            }
            else {
                p_info.not_matchable_with.insert(j); /* no need to match it with scanner j */
            }
        }

        //throw std::exception("queue implementation is needed to return back to this scanner and check it again");
        return false;
    }


    bool check_overlap_in_rotations(const std::set<point>& coord1, const scanner_info& coord2) {
        if (check_and_register_overlap(coord1, coord2.coordinates)) {
            return true;
        }

        /* need to consider all rotations */
        for (const auto& coord2_rotation : coord2.rotations) {
            if (check_and_register_overlap(coord1, coord2_rotation)) {
                return true;
            }
        }

        return false;
    }


    void save_aligned_scanner(const point& p_scanner, const std::set<point>& p_beacons) {
        m_map.push_back({ p_scanner, p_beacons });
        m_beacons.insert(p_beacons.begin(), p_beacons.end());
    }


    using appreance_t = std::unordered_map<int, std::unordered_set<std::size_t>>;


    int get_offset(const appreance_t& p_appearance) {
        std::size_t count_appearance = 0;
        int offset = INT_MAX;

        for (const auto& pair : p_appearance) {
            if (pair.second.size() > count_appearance) {
                count_appearance = pair.second.size();
                offset = pair.first;
            }
        }

        if (count_appearance >= 12) {
            return offset;
        }

        return INT_MAX;
    }


    bool check_and_register_overlap(const std::set<point>& coord1, const std::set<point>& coord2) {
        appreance_t x_appearance; /* offset : which points */
        appreance_t y_appearance;
        appreance_t z_appearance;

        for (const auto & p_point1 : coord1) {
            std::size_t index = 0;
            for (const auto& p_point2 : coord2) {
                const point difference = p_point1 - p_point2;
                x_appearance[difference.x].insert(index);
                y_appearance[difference.y].insert(index);
                z_appearance[difference.z].insert(index);

                index++;
            }
        }


        int x_offset = get_offset(x_appearance);
        int y_offset = get_offset(y_appearance);
        int z_offset = get_offset(z_appearance);

        if (x_offset != INT_MAX && y_offset != INT_MAX && z_offset != INT_MAX) {
            const point offset = { x_offset, y_offset, z_offset };
            const std::set<point> aligned_coord2 = align_coordinates(coord2, { x_offset, y_offset, z_offset });

            save_aligned_scanner(offset, aligned_coord2);
            return true;
        }

        return false;
    }


    std::set<point> align_coordinates(const std::set<point>& p_coord, const point& p_offset) const {
        std::set<point> result;

        for (const point& p : p_coord) {
            result.insert(p + p_offset);
        }

        return result;
    }
};


std::vector<std::set<point>> read_scanners_and_beacons() {
    std::ifstream stream("input.txt");

    std::vector<std::set<point>> result;
    std::set<point> scanner_data;

    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            result.push_back(std::move(scanner_data));
            continue;
        }

        if (line.substr(0, 3) == "---") {
            continue;
        }

        std::regex pattern("(\\S+),(\\S+),(\\S+)");
        std::smatch groups;

        if (!std::regex_search(line, groups, pattern)) {
            throw std::invalid_argument("incorrect file format");
        }

        point p = { std::stoi(groups[1].str()), std::stoi(groups[2].str()), std::stoi(groups[3].str()) };
        scanner_data.insert(p);
    }

    if (!scanner_data.empty()) {
        result.push_back(scanner_data);
    }

    return result;
}


int main() {
    const auto input = read_scanners_and_beacons();

    matcher map_builder;
    map_builder.build_map(input);

    std::cout << "The amount of beacons: " << map_builder.get_beacons_amount() << std::endl;

    return 0;
}
