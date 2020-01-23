#include <algorithm>
#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


using ascii_map = std::vector<std::string>;


ascii_map read_map() {
    ascii_map result;

    std::ifstream stream("input.txt");
    std::string map_line;

    while (std::getline(stream, map_line)) {
        result.push_back(map_line);
    }

    stream.close();
    return result;
}


struct position {
public:
    int x = -1;
    int y = -1;

public:
    bool operator==(const position& p_other) const {
        return (x == p_other.x) && (y == p_other.y);
    }

    operator bool() const {
        return (x != -1) && (y != -1);
    }
};


class portal {
public:
    using ptr = std::shared_ptr<portal>;

private:
    position m_entrance1;
    position m_entrance2;

    std::string m_id;

public:
    portal(const std::string& p_id, const position & p_entrance1, const position & p_entrance2) :
        m_id(p_id), m_entrance1(p_entrance1), m_entrance2(p_entrance2)
    { }

public:
    void assign_entrance2(const position& p_entrance2) {
        m_entrance2 = p_entrance2;
    }

    const position& get_entrance1() const {
        return m_entrance1;
    }

    const position& get_entrance2() const {
        return m_entrance2;
    }

    const std::string& get_id() const {
        return m_id;
    }

    bool is_start() const {
        return m_id == "AA";
    }

    bool is_finish() const {
        return m_id == "ZZ";
    }

    position enter(const position& p_pos) const {
        if (p_pos == m_entrance1) {
            return m_entrance2;
        }
        else if (p_pos == m_entrance2) {
            return m_entrance1;
        }
        else {
            throw std::exception("Impossible to go through the portal because there is no portal.");
        }
    }
};


class portal_container {
private:
    const ascii_map& m_map;

    std::unordered_map<int, std::unordered_map<int, bool>> m_visited;
    std::unordered_map<int, std::unordered_map<int, portal::ptr>> m_entrances;
    std::unordered_map<std::string, portal::ptr> m_portals;

public:
    portal_container(const ascii_map& map) : m_map(map) {
        initialize();
    }

public:
    position get_start() {
        return m_portals["AA"]->get_entrance1();
    }

    position get_finish() {
        return m_portals["ZZ"]->get_entrance1();
    }

    bool is_entrance(const position& p_pos) {
        auto iter_y = m_entrances.find(p_pos.y);
        if (iter_y == m_entrances.end()) {
            return false;
        }

        auto iter_x = iter_y->second.find(p_pos.x);
        if (iter_x == iter_y->second.end()) {
            return false;
        }

        if (iter_x->second->is_start() || iter_x->second->is_finish()) {
            return false;
        }

        return true;
    }

    position go_through_entrance(const position& p_pos) {
        return m_entrances[p_pos.y][p_pos.x]->enter(p_pos);
    }

    void print() {
        for (auto& elem : m_portals) {
            std::cout << elem.second->get_id() << std::endl;
        }
    }

private:
    void initialize() {
        for (int y = 0; y < m_map.size(); y++) {
            for (int x = 0; x < m_map[y].size(); x++) {
                const char symbol = m_map[y][x];
                if (isalpha(symbol)) {
                    expand_portal(x, y);
                }
            }
        }
    }

    void expand_portal(const int x, const int y) {
        auto iter_y = m_visited.find(y);
        if (iter_y != m_visited.end()) {
            auto iter_x = iter_y->second.find(x);
            if (iter_x != iter_y->second.end()) {
                return;     /* already expanded - no need to analyse portal */
            }
        }

        const position first_part_pos = { x, y };
        const position second_part_pos = get_second_part(x, y);
        const position entrance_pos = get_entrance(first_part_pos, second_part_pos);

        m_visited[first_part_pos.y][first_part_pos.x] = true;
        m_visited[second_part_pos.y][second_part_pos.x] = true;

        /* check if another side of the portal is already exist */
        std::string id = std::string(1, m_map[first_part_pos.y][first_part_pos.x]) + std::string(1, m_map[second_part_pos.y][second_part_pos.x]);
        auto iter_portal = m_portals.find(id);
        if (iter_portal != m_portals.end()) {
            iter_portal->second->assign_entrance2(entrance_pos);
            m_entrances[entrance_pos.y][entrance_pos.x] = iter_portal->second;
            return;
        }

        /* if there is no any side of the portal - then create a new one */
        portal::ptr portal_instance = std::make_shared<portal>(id, entrance_pos, position());
        m_entrances[entrance_pos.y][entrance_pos.x] = portal_instance;
        m_portals[id] = portal_instance;
    }

    position get_entrance(const position& p_p1, const position& p_p2) {
        if (p_p1.x == p_p2.x) {
            if (p_p1.y > p_p2.y) {
                return get_x_entrance(p_p1, p_p2);
            }
            else {
                return get_x_entrance(p_p2, p_p1);
            }
        }

        if (p_p1.y == p_p2.y) {
            if (p_p1.x > p_p2.x) {
                return get_y_entrance(p_p1, p_p2);
            }
            else {
                return get_y_entrance(p_p2, p_p1);
            }
        }

        throw std::exception("Impossible to entrance for the portal.");
    }

    position get_x_entrance(const position& p_p1, const position& p_p2) {
        if (is_way(p_p1.x, p_p1.y + 1)) {
            return { p_p1.x, p_p1.y + 1 };
        }

        if (is_way(p_p1.x, p_p2.y - 1)) {
            return { p_p1.x, p_p2.y - 1 };
        }

        throw std::exception("Impossible to find a 'x' way for the portal is not found.");
    }

    position get_y_entrance(const position& p_p1, const position& p_p2) {
        if (is_way(p_p1.x + 1, p_p1.y)) {
            return { p_p1.x + 1, p_p1.y };
        }

        if (is_way(p_p2.x - 1, p_p1.y)) {
            return { p_p2.x - 1, p_p1.y };
        }

        throw std::exception("Impossible to find a 'y' way for the portal is not found.");
    }

    position get_second_part(const int x, const int y) {
        if (is_portal_part(x - 1, y)) { return { x - 1, y }; }
        if (is_portal_part(x + 1, y)) { return { x + 1, y }; }
        if (is_portal_part(x, y - 1)) { return { x, y - 1 }; }
        if (is_portal_part(x, y + 1)) { return { x, y + 1 }; }

        throw std::exception("Impossible to find the second part of the portal.");
    }

    bool is_way(const int x, const int y) {
        return is_object_checker(x, y, [](const char symbol) {
            return symbol == '.';
        });
    }

    bool is_portal_part(const int x, const int y) {
        return is_object_checker(x, y, [](const char symbol) {
            return std::isalpha(symbol);
        });
    }

    bool is_object_checker(const int x, const int y, const std::function<bool(const char)>& p_rule) {
        if (y < 0 || y >= m_map.size()) { return false; }
        if (x < 0 || x >= m_map[y].size()) { return false; }

        const char symbol = m_map[y][x];
        if (p_rule(symbol)) {
            return true;
        }

        return false;
    }
};


class path_finder {
private:
    ascii_map m_map;
    std::vector<std::vector<bool>> m_visited;
    portal_container m_portals;

public:
    path_finder() : 
        m_map(read_map()), 
        m_portals(m_map),
        m_visited(m_map.size(), std::vector<bool>(m_map.front().size(), false))
    { }

public:
    int find_closest_path() {
        int length = 0;

        std::list<position> cur_level;
        std::list<position> next_level;

        next_level.push_back(m_portals.get_start());
        m_visited[next_level.front().y][next_level.front().x] = true;

        while (!next_level.empty()) {
#if 0
            print();
            std::getchar();
#endif

            for (auto& cur : next_level)
            {
                /* check if we reach the finish */
                if (m_portals.get_finish() == cur) {
                    return length;
                }

                /* check if it is an entrance to the portal */
                if (m_portals.is_entrance(cur)) {
                    position gate_position = m_portals.go_through_entrance(cur);

                    /* check if we were there before */
                    append_position_if_acceptable(gate_position, cur_level);
                }

                /* append non-portal ways */
                get_valid_next_positions(cur, cur_level);
            }

            next_level = std::move(cur_level);

            length++;
        }

        throw std::exception("Impossible to reach the finish.");
    }

private:
    void get_valid_next_positions(const position& p_pos, std::list<position> & p_next) {
        append_position_if_acceptable({ p_pos.x - 1, p_pos.y }, p_next);
        append_position_if_acceptable({ p_pos.x + 1, p_pos.y }, p_next);
        append_position_if_acceptable({ p_pos.x, p_pos.y - 1 }, p_next);
        append_position_if_acceptable({ p_pos.x, p_pos.y + 1 }, p_next);
    }

    void append_position_if_acceptable(const position& p_candidate, std::list<position>& p_next) {
        if (is_valid_position(p_candidate)) {
            p_next.push_back(p_candidate);
        }

        m_visited[p_candidate.y][p_candidate.x] = true;
    }

    bool is_valid_position(const position& p_pos) {
        if (p_pos.y < 0 && p_pos.y >= m_map.size()) {
            return false;
        }

        if (p_pos.x < 0 && p_pos.x >= m_map.at(p_pos.y).size()) {
            return false;
        }

        if (m_visited[p_pos.y][p_pos.x]) {
            return false;
        }

        if (m_map[p_pos.y][p_pos.x] == '.') {
            return true;
        }

        return false;
    }

    void print() {
        for (int y = 0; y < m_map.size(); y++) {
            for (int x = 0; x < m_map.front().size(); x++) {
                if ((m_visited[y][x] == true) && (m_map[y][x] == '.')) {
                    std::cout << '@';
                }
                else {
                    std::cout << m_map[y][x];
                }
            }

            std::cout << std::endl;
        }

        std::cout << std::endl << std::endl;
    }
};


int main() {
    path_finder searcher;
    std::cout << "The shortest path is: " << searcher.find_closest_path() << std::endl;

    return 0;
}
