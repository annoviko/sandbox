#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
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
    const position & get_position() const {
        return m_position;
    }

    position_sequence get_neighbors() const {
        return {
            { m_position.x - 1, m_position.y - 1 },
            { m_position.x - 1, m_position.y + 1 },
            { m_position.x + 1, m_position.y - 1 },
            { m_position.x + 1, m_position.y + 1 },
            { m_position.x - 2, m_position.y },
            { m_position.x + 2, m_position.y },
        };
    }

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
    using tile_floor = std::unordered_map<std::int64_t, std::unordered_map<std::int64_t, tile>>;

    using neighbor_filter = std::function<bool(const tile & p_tile)>;

private:
    tile_floor m_map;

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


    void simulate_evolution(const std::size_t p_days) {
        for (std::size_t i = 0; i < p_days; i++) {
            simulate();
            std::cout << "Day " << i + 1 << ": " << count_black() << std::endl;
        }
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

private:
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


    std::size_t count_black_neighbors(const tile & p_tile) {
        return count_neighbors(p_tile, [](const tile & p_tile) {
            return p_tile.is_black();
        });
    }


    std::size_t count_white_neighbors(const tile & p_tile) {
        return count_neighbors(p_tile, [](const tile & p_tile) { 
            return p_tile.is_white();
        });
    }


    std::size_t count_neighbors(const tile & p_tile, const neighbor_filter & p_filter) {
        std::size_t result = 0;

        position_sequence coordinates = p_tile.get_neighbors();
        for (auto & coordinate : coordinates) {
            tile & neighbor = m_map[coordinate.y][coordinate.x];
            if (p_filter(neighbor)) {
                result++;
            }
        }

        return result;
    }


    void scan_tile_neighbors(const tile & p_tile, tile_floor & p_floor) {
        if (p_tile.is_white()) {
            return; /* analyse only black tile */
        }

        /* check current tile itself */
        if (count_black_neighbors(p_tile) == 2) {   /* with exactly 2 black tiles */
            position coordinate = { p_tile.get_position().x, p_tile.get_position().y };
            p_floor[coordinate.y][coordinate.x] = tile(color::WHITE, coordinate);    /* set tile to white color */
        }

        position_sequence coordinates = p_tile.get_neighbors();     /* get all neighbors of black tile */
        for (auto & coordinate : coordinates) {
            tile & neighbor = m_map[coordinate.y][coordinate.x];

            const std::size_t black_neighbors = count_black_neighbors(neighbor);

            if (neighbor.is_white()) {  /* if white tile */
                if (black_neighbors == 0 || black_neighbors > 2) {     /* with zero or more than 2 black tiles */
                    p_floor[coordinate.y][coordinate.x] = tile(color::BACK, coordinate);    /* set tile to black color */
                }
            }
            else {  /* if black tile */
                if (black_neighbors == 2) {     /* with exactly 2 black tiles */
                    p_floor[coordinate.y][coordinate.x] = tile(color::WHITE, coordinate);    /* set tile to white color */
                }
            }
        }
    }


    void simulate() {
        tile_floor next_floor = m_map;

        for (const auto & row : m_map) {
            for (const auto & tile : row.second) {
                scan_tile_neighbors(tile.second, next_floor);
            }
        }

        m_map = std::move(next_floor);
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
    auto pathes = read("test.txt");

    hotel_floor instance;
    instance.identify_sequence(pathes);

    std::cout << "Black tiles on the floor: " << instance.count_black() << std::endl;

    instance.simulate_evolution(100);

    std::cout << "Black tiles on the floor (after 100 days): " << instance.count_black() << std::endl;

    return 0;
}
