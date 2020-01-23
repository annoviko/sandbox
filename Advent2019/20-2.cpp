#include <algorithm>
#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <set>
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

    portal::ptr get_portal(const position& p_pos) {
        return m_entrances[p_pos.y][p_pos.x];
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


struct maze_context {
public:
    std::vector<std::vector<bool>> m_visited;

public:
    maze_context(const std::size_t p_rows, const std::size_t p_cols) :
        m_visited(p_rows, std::vector<bool>(p_cols, false))
    { }
};


struct cursor_context {
public:
    position m_position;
    std::set<std::string> m_portals;

public:
    cursor_context(const position& p_position, const std::set<std::string>& p_portals) :
        m_position(p_position),
        m_portals(p_portals)
    { }

public:
    std::size_t get_level() const {
        return m_portals.size();
    }

    void visit(const std::string& p_portal_id) {
        auto iter = m_portals.find(p_portal_id);
        if (iter != m_portals.end()) {
            m_portals.erase(iter);
        }
        else {
            m_portals.insert(p_portal_id);
        }
    }
};


class path_finder {
private:
    ascii_map m_map;
    portal_container m_portals;

    std::vector<maze_context> m_context;


public:
    path_finder() :
        m_map(read_map()),
        m_portals(m_map),
        m_context(1, { m_map.size(), m_map.front().size() })
    { }

public:
    int find_closest_path() {
        int length = 0;

        std::list<cursor_context> current_movement;
        std::list<cursor_context> next_movement;

        next_movement.push_back({ m_portals.get_start(), { } });
        m_context[next_movement.front().get_level()].m_visited[next_movement.front().m_position.y][next_movement.front().m_position.x] = true;

        while (!next_movement.empty()) {
            for (auto& cur : next_movement)
            {
                /* check if we reach the finish */
                if ((cur.get_level() == 0) && (m_portals.get_finish() == cur.m_position)) {
                    return length;
                }

                /* check if it is an entrance to the portal */
                if (m_portals.is_entrance(cur.m_position)) {
                    position gate_position = m_portals.go_through_entrance(cur.m_position);

                    /* check if we were there before */
                    cursor_context candidate_context = { gate_position, cur.m_portals };
                    candidate_context.visit(m_portals.get_portal(cur.m_position)->get_id());

                    /* check if there is no level that may appear */
                    if (candidate_context.get_level() >= m_context.size()) {
                        m_context.push_back({ m_map.size(), m_map.front().size() });
                        m_context[candidate_context.get_level()].m_visited[candidate_context.m_position.y][candidate_context.m_position.x] = true;
                    }

                    append_position_if_acceptable(candidate_context, current_movement);
                }

                /* append non-portal ways */
                get_valid_next_positions(cur, current_movement);
            }

            next_movement = std::move(current_movement);
            length++;
        }

        throw std::exception("Impossible to reach the finish.");
    }

private:
    void get_valid_next_positions(const cursor_context& p_cur, std::list<cursor_context> & p_next_movement) {
        append_position_if_acceptable(cursor_context({ p_cur.m_position.x - 1, p_cur.m_position.y }, { }), p_next_movement);
        append_position_if_acceptable(cursor_context({ p_cur.m_position.x + 1, p_cur.m_position.y }, { }), p_next_movement);
        append_position_if_acceptable(cursor_context({ p_cur.m_position.x, p_cur.m_position.y - 1 }, { }), p_next_movement);
        append_position_if_acceptable(cursor_context({ p_cur.m_position.x, p_cur.m_position.y + 1 }, { }), p_next_movement);
    }

    void append_position_if_acceptable(const cursor_context& p_candidate, std::list<cursor_context>& p_next_movement) {
        if (is_valid_position(p_candidate)) {
            p_next_movement.push_back(p_candidate);
        }

        const std::size_t maze_level = p_candidate.get_level();
        auto& maze_level_context = m_context[maze_level];

        maze_level_context.m_visited[p_candidate.m_position.y][p_candidate.m_position.x] = true;
    }

    bool is_valid_position(const cursor_context& p_cursor) {
        if (p_cursor.m_position.y < 0 && p_cursor.m_position.y >= m_map.size()) {
            return false;
        }

        if (p_cursor.m_position.x < 0 && p_cursor.m_position.x >= m_map.at(p_cursor.m_position.y).size()) {
            return false;
        }

        const std::size_t maze_level = p_cursor.get_level();
        const auto& maze_level_context = m_context[maze_level];
        if (maze_level_context.m_visited[p_cursor.m_position.y][p_cursor.m_position.x]) {
            return false;
        }

        if (m_map[p_cursor.m_position.y][p_cursor.m_position.x] == '.') {
            return true;
        }

        return false;
    }
};


int main() {
    path_finder searcher;
    std::cout << "The shortest path is: " << searcher.find_closest_path() << std::endl;

    return 0;
}