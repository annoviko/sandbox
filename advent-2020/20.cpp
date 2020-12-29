#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#define NOT_ASSIGNED    INT_MAX


enum class e_border {
    UPPER = 0, RIGHT, LOWER, LEFT
};


struct tile {
public:
    using ptr = std::shared_ptr<tile>;

public:
    std::size_t m_id = 0;
    std::vector<std::string> m_borders;
    std::vector<std::string> m_content;

    int m_i = INT_MAX;
    int m_j = INT_MAX;

public:
    void rotate() {
        std::vector<std::string> result = m_content;
        for (std::size_t i = 0; i < m_content.size(); i++) {
            for (std::size_t j = 0; j < m_content[0].size(); j++) {
                result[j][m_content.size() - i - 1] = m_content[i][j];
            }
        }
        m_content = std::move(result);

        rotate_borders();
    }

    void flip() {
        for (auto & row : m_content) {
            std::reverse(row.begin(), row.end());
        }

        flip_borders();
    }

    void print() {
        for (auto & row : m_content) {
            std::cout << row << std::endl;
        }
    }

    void print_with_borders() {
        print();

        std::cout << "UPPER: " << m_borders[(int)e_border::UPPER] << std::endl;
        std::cout << "RIGHT: " << m_borders[(int)e_border::RIGHT] << std::endl;
        std::cout << "LOWER: " << m_borders[(int)e_border::LOWER] << std::endl;
        std::cout << "LEFT:  " << m_borders[(int)e_border::LEFT] << std::endl;
    }

private:
    void rotate_borders() {
        std::vector<std::string> new_borders(4);

        new_borders[(int)e_border::UPPER] = m_borders[(int)e_border::LEFT];
        new_borders[(int)e_border::RIGHT] = m_borders[(int)e_border::UPPER];
        new_borders[(int)e_border::LOWER] = m_borders[(int)e_border::RIGHT];
        new_borders[(int)e_border::LEFT] = m_borders[(int)e_border::LOWER];

        m_borders = std::move(new_borders);
    }

    void flip_borders() {
        std::reverse(m_borders[(int)e_border::UPPER].begin(), m_borders[(int)e_border::UPPER].end());
        std::reverse(m_borders[(int)e_border::LOWER].begin(), m_borders[(int)e_border::LOWER].end());
        std::reverse(m_borders[(int)e_border::RIGHT].begin(), m_borders[(int)e_border::RIGHT].end());
        std::reverse(m_borders[(int)e_border::LEFT].begin(), m_borders[(int)e_border::LEFT].end());
        std::swap(m_borders[(int)e_border::RIGHT], m_borders[(int)e_border::LEFT]);
    }
};


using tile_sequence = std::list<tile::ptr>;


enum class read_state { HEADER, CONTENT };


void fill_borders(tile_sequence & p_seq) {
    for (auto & instance : p_seq) {
        instance->m_borders.resize(4);

        std::string upper_border = instance->m_content.front();
        std::string lower_border = instance->m_content.back();
        std::reverse(lower_border.begin(), lower_border.end());

        instance->m_borders[(std::size_t) e_border::UPPER] = upper_border;
        instance->m_borders[(std::size_t) e_border::LOWER] = lower_border;

        std::string lborder, rborder;
        for (auto & row : instance->m_content) {
            lborder.push_back(row.front()); /* LEFT_BORDER */
            rborder.push_back(row.back());  /* RIGHT_BORDER */
        }

        std::reverse(lborder.begin(), lborder.end());

        instance->m_borders[(std::size_t) e_border::LEFT] = lborder; /* LEFT_BORDER */
        instance->m_borders[(std::size_t) e_border::RIGHT] = rborder; /* RIGHT_BORDER */
    }
}


tile_sequence read(const std::string & p_file) {
    const static std::regex header_pattern("Tile (\\d+):");

    tile_sequence result;
    tile::ptr current = std::make_shared<tile>();
    read_state state = read_state::HEADER;

    std::ifstream stream(p_file);
    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            result.push_back(current);
            current = std::make_shared<tile>();

            state = read_state::HEADER;
            continue;
        }

        switch (state) {
        case read_state::HEADER:
            {
                std::smatch groups;
                if (std::regex_match(line, groups, header_pattern)) {
                    current->m_id = std::stoull(groups[1]);
                }
                else {
                    throw std::invalid_argument("Invalid pattern is detected.");
                }

                state = read_state::CONTENT;
                break;
            }

        case read_state::CONTENT:
            current->m_content.push_back(line);
            break;

        default:
            break;
        }
    }

    result.push_back(current);  /* append the last tile */
    fill_borders(result);

    return result;
}


using tile_row = std::unordered_map<int, tile::ptr>;
using tile_map = std::unordered_map<int, tile_row>;


struct position {
    int m_i = 0;
    int m_j = 0;
};


struct neighbor_descriptor {
    position m_pos;
    e_border m_border;
};


class tile_matcher {
private:
    enum e_instruction { NOP, ROTATE, FLIP };

private:
    tile_map m_map;
    int m_upper = 0, m_lower = 0, m_left = 0, m_right = 0;

    std::list<tile::ptr> m_not_placed_tiles;

    /*                                                   0     90      180    270     0      90      180     270     */
    const std::vector<e_instruction> m_instructions = { NOP, ROTATE, ROTATE, ROTATE, FLIP, ROTATE, ROTATE, ROTATE };

public:
    tile_matcher(const tile_sequence & p_seq) {
        auto initial = *p_seq.begin();
        initial->m_i = 0;
        initial->m_j = 0;

        m_map[initial->m_i][initial->m_j] = initial;

        for (auto iter = std::next(p_seq.begin()); iter != p_seq.end(); iter++) {
            m_not_placed_tiles.push_back(*iter);
        }

        std::list<tile::ptr> to_consider = { initial };
        while (!to_consider.empty()) {
            tile::ptr current = to_consider.front();
            to_consider.pop_front();

            std::size_t amount_neighbors = 0;
            for (std::size_t i = 0; i < 4; i++) {
                e_border border_id = (e_border) i;

                /* check it already has neighbors */
                if (has_neighbor(current, border_id)) {
                    amount_neighbors++;
                    continue;
                }

                tile::ptr neighbor = find_neighbor(current, border_id);

                /* no neighbors for this border - probably image border or image corner */
                if (neighbor == nullptr) {
                    continue;
                }

                to_consider.push_back(neighbor);
                assign_neighbor(current, border_id, neighbor);
                amount_neighbors++;

                std::cout << "After Iteration:" << std::endl;
                print();
                std::cout << std::endl;
            }

            if (amount_neighbors < 2) {
                throw std::exception("Impossible to find enough neighbors.");
            }
        }
    }

    void print() {
        for (int i = m_lower; i <= m_upper; i++) {
            for (int j = m_left; j <= m_right; j++) {
                auto current = m_map[i][j];
                std::cout << ((current != nullptr) ? current->m_id : 0) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::size_t get_border_multiply_id() {
        return 0;
    }

private:
    void assign_neighbor(const tile::ptr & p_current, const e_border & p_border, const tile::ptr & p_neighbor) {
        switch (p_border) {
        case e_border::UPPER:
            m_map[p_current->m_i - 1][p_current->m_j] = p_neighbor;
            p_neighbor->m_i = p_current->m_i - 1;
            p_neighbor->m_j = p_current->m_j;

            m_lower = std::min(p_neighbor->m_i, m_lower);

            break;

        case e_border::LOWER:
            m_map[p_current->m_i + 1][p_current->m_j] = p_neighbor;
            p_neighbor->m_i = p_current->m_i + 1;
            p_neighbor->m_j = p_current->m_j;

            m_upper = std::max(p_neighbor->m_i, m_upper);

            break;

        case e_border::LEFT:
            m_map[p_current->m_i][p_current->m_j - 1] = p_neighbor;
            p_neighbor->m_i = p_current->m_i;
            p_neighbor->m_j = p_current->m_j - 1;

            m_left = std::min(p_neighbor->m_j, m_left);

            break;

        case e_border::RIGHT:
            m_map[p_current->m_i][p_current->m_j + 1] = p_neighbor;
            p_neighbor->m_i = p_current->m_i;
            p_neighbor->m_j = p_current->m_j + 1;

            m_right = std::max(p_neighbor->m_j, m_right);

            break;

        default:
            //throw std::exception("Unexpected border type.");
        }
    }


    bool has_neighbor(const tile::ptr & p_current, const e_border & p_border) {
        switch (p_border) {
        case e_border::UPPER:
            return m_map[p_current->m_i - 1][p_current->m_j] != nullptr;

        case e_border::LOWER:
            return m_map[p_current->m_i + 1][p_current->m_j] != nullptr;

        case e_border::LEFT:
            return m_map[p_current->m_i][p_current->m_j - 1] != nullptr;

        case e_border::RIGHT:
            return m_map[p_current->m_i][p_current->m_j + 1] != nullptr;

        default:
            throw std::exception("Unexpected border type.");
        }
    }


    tile::ptr find_neighbor(const tile::ptr & p_current, const e_border p_border_id) {
        for (auto iter = m_not_placed_tiles.begin(); iter != m_not_placed_tiles.end(); iter++) {
            auto candidate = *iter;

            for (auto instruction : m_instructions) {
                switch (instruction) {
                case NOP:
                    break;

                case FLIP:
                    candidate->flip();
                    break;

                case ROTATE:
                    candidate->rotate();
                    break;

                default:
                    throw std::exception("Unknown type of operation.");
                }

                if (is_match(p_current, p_border_id, candidate)) {
                    auto candidate_neighbors = get_candidate_neighbors(p_current, p_border_id, candidate);
                    if (is_matched_with_neighbors(candidate, candidate_neighbors)) {
                        m_not_placed_tiles.erase(iter);
                        return candidate;
                    }
                }
            }
        }

        return nullptr;
    }


    position get_candidate_position(const tile::ptr & p_current, const e_border p_border_id, const tile::ptr & p_cadidante) {
        position result;

        switch (p_border_id) {
        case e_border::UPPER:
            result = { p_current->m_i - 1, p_current->m_j };
            break;

        case e_border::RIGHT:
            result = { p_current->m_i, p_current->m_j + 1 };
            break;

        case e_border::LOWER:
            result = { p_current->m_i + 1, p_current->m_j };
            break;

        case e_border::LEFT:
            result = { p_current->m_i, p_current->m_j - 1 };
            break;

        default:
            throw std::exception("Unexpected border type.");
        }

        return result;
    }


    e_border get_opposite_border(const e_border p_border_id) {
        switch (p_border_id) {
        case e_border::LEFT: return e_border::RIGHT;
        case e_border::RIGHT: return e_border::LEFT;
        case e_border::UPPER: return e_border::LOWER;
        case e_border::LOWER: return e_border::UPPER;
        default: throw std::exception("Unexpected border type.");
        }
    }


    std::list<neighbor_descriptor> get_candidate_neighbors(const tile::ptr & p_current, const e_border p_border_id, const tile::ptr & p_cadidante) {
        std::list<neighbor_descriptor> result;

        e_border candidate_atteched_border = get_opposite_border(p_border_id);
        position candidate_position = get_candidate_position(p_current, p_border_id, p_cadidante);
        for (std::size_t i = 0; i < 4; i++) {
            e_border candidate_border = (e_border)i;

            if (candidate_atteched_border == candidate_border) {
                continue;   /* do not consider attached border neighbor - it is matching */
            }

            switch (candidate_border) {
            case e_border::UPPER:
                result.push_back({ { candidate_position.m_i - 1, candidate_position.m_j }, e_border::UPPER });
                break;

            case e_border::LOWER:
                result.push_back({ { candidate_position.m_i + 1, candidate_position.m_j }, e_border::LOWER });
                break;

            case e_border::LEFT:
                result.push_back({ { candidate_position.m_i, candidate_position.m_j - 1 }, e_border::LEFT });
                break;

            case e_border::RIGHT:
                result.push_back({ { candidate_position.m_i, candidate_position.m_j + 1 }, e_border::RIGHT });
                break;
            }
        }

        return result;
    }


    bool is_matched_with_neighbors(const tile::ptr & p_cadidante, const std::list<neighbor_descriptor> & candidate_neigbors) {
        for (auto & neighbor : candidate_neigbors) {
            auto neighbor_tile = m_map[neighbor.m_pos.m_i][neighbor.m_pos.m_j];
            if (neighbor_tile == nullptr) {
                continue;
            }

            if (!is_match(p_cadidante, neighbor.m_border, neighbor_tile)) {
                return false;
            }
        }

        return true;
    }


    bool is_match(const tile::ptr & p_current, const e_border p_border_id, const tile::ptr & p_cadidante) {
        switch (p_border_id) {
        case e_border::LEFT:
            return p_current->m_borders[(int)p_border_id] == p_cadidante->m_borders[(int)e_border::RIGHT];

        case e_border::RIGHT:
            return p_current->m_borders[(int)p_border_id] == p_cadidante->m_borders[(int)e_border::LEFT];

        case e_border::UPPER:
            return p_current->m_borders[(int)p_border_id] == p_cadidante->m_borders[(int)e_border::LOWER];

        case e_border::LOWER:
            return p_current->m_borders[(int)p_border_id] == p_cadidante->m_borders[(int)e_border::UPPER];

        default:
            throw std::exception("Unexpected border type.");
        }
    }
};


int main() {
    auto tiles = read("test.txt");
    //auto tile = *tiles.begin();
   
    //tile->print_with_borders();
    //std::cout << "\n\n- [ROTATE]:" << std::endl;
    //tile->rotate();
    //tile->print_with_borders();
    //std::cout << "\n\n- [FLIP]:" << std::endl;
    //tile->flip();
    //tile->print_with_borders();

    tile_matcher(tiles).print();
    //std::cout << "The multiply ID of borders: " << tile_matcher(tiles).get_border_multiply_id() << std::endl;

    return 0;
}
