#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <unordered_map>


constexpr static std::int64_t INVALID_VALUE = std::numeric_limits<std::int64_t>::max();


enum class color { WHITE = 0, BACK };


enum class direction { NW = 0, SW, NE, SE, W, E };


struct position {
    std::int64_t x = INVALID_VALUE;
    std::int64_t y = INVALID_VALUE;
};


using position_sequence = std::vector<position>;

using direction_sequence = std::vector<direction>;

using tile_path_sequence = std::vector<direction_sequence>;


class tile {
private:
    color    m_color = color::WHITE;
    position m_position;

public:
    tile() = default;

    tile(const color & p_color, const position & p_position) :
        m_color(p_color),
        m_position(p_position)
    { }

public:
    position get_neighbor(const direction p_direction) const {
        position result = m_position;

        switch (p_direction) {
        case direction::NW:
            result.x--;
            result.y--;
            break;

        case direction::SW:
            result.x--;
            result.y++;
            break;

        case direction::NE:
            result.x++;
            result.y--;
            break;

        case direction::SE:
            result.x++;
            result.y++;
            break;

        case direction::W:
            result.x -= 2;
            break;

        case direction::E:
            result.x += 2;
            break;

        default:
            throw std::exception("Not supported direction");
        }

        return result;
    }

    void flip() {
        switch (m_color) {
        case color::WHITE:
            m_color = color::BACK;
            break;

        case color::BACK:
            m_color = color::WHITE;
            break;

        default:
            throw std::exception("Not supported color.");
        }
    }

    void set_position(const position & p_position) {
        m_position = p_position;
    }

    bool is_black() const { return m_color == color::BACK; }

    bool is_white() const { return m_color == color::WHITE; }
};


class hotel_floor {
private:
    std::unordered_map<std::int64_t, std::unordered_map<std::int64_t, tile>> m_map;

public:
    hotel_floor() {
        m_map[0][0] = tile(color::WHITE, { 0, 0 });
    }

public:
    void identify_sequence(const tile_path_sequence & p_pathes) {
        for (const auto & path : p_pathes) {
            identify(path);
        }
    }

    void identify(const direction_sequence & p_path) {
        tile * cur = &(m_map[0][0]);
        position next_pos = { 0, 0 };

        for (const auto dir : p_path) {
            next_pos = cur->get_neighbor(dir);
            cur = &(m_map[next_pos.y][next_pos.x]);
            cur->set_position(next_pos);
        }

        cur->flip();
    }

    std::size_t count_black() const {
        std::size_t result = 0;

        for (const auto & row : m_map) {
            for (const auto & tile : row.second) {
                if (tile.second.is_black()) {
                    result++;
                }
            }
        }

        return result;
    }
};


tile_path_sequence read(const std::string & p_file) {
    tile_path_sequence result;

    std::fstream stream(p_file);

    for (std::string line; std::getline(stream, line);) {
        direction_sequence tile_path;

        for (std::size_t lborder = 0; lborder < line.size();) {
            std::size_t rborder = lborder + 2;

            std::string dir_code;
            if (rborder <= line.size()) {
                dir_code = line.substr(lborder, 2);
            }
            else {
                rborder--;
                dir_code = line.substr(lborder, 1);
            }

            if (dir_code == "nw") {
                tile_path.push_back(direction::NW);
            }
            else if (dir_code == "sw") {
                tile_path.push_back(direction::SW);
            }
            else if (dir_code == "ne") {
                tile_path.push_back(direction::NE);
            }
            else if (dir_code == "se") {
                tile_path.push_back(direction::SE);
            }
            else if (dir_code[0] == 'w') {
                if (dir_code.size() == 2) {
                    rborder--;
                }
                tile_path.push_back(direction::W);
            }
            else if (dir_code[0] == 'e') {
                if (dir_code.size() == 2) {
                    rborder--;
                }
                tile_path.push_back(direction::E);
            }
            else {
                throw std::exception("Unknown direction in the input file.");
            }

            lborder = rborder;
        }

        result.push_back(tile_path);
    }

    return result;
}


int main() {
    auto pathes = read("input.txt");

    hotel_floor instance;
    instance.identify_sequence(pathes);

    std::cout << "Black tiles on the floor: " << instance.count_black() << std::endl;

    return 0;
}
