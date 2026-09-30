#include <iostream>
#include <fstream>
#include <regex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


struct node_t {
    std::string name;
    int flow = 0;
    std::vector<std::string> neis;
};


using graph_t = std::vector<node_t>;


graph_t read_input() {
    std::ifstream stream("input.txt");

    graph_t g;
    for (std::string line; std::getline(stream, line);) {
        auto begin = line.find(' ') + 1;

        std::string from = line.substr(begin, 2);

        begin = line.find('=', begin + 2) + 1;
        auto end = line.find(';', begin + 1);

        const std::string flow_str = line.substr(begin, end - begin);
        int flow = std::stoi(flow_str);

        begin = line.find("valve", end + 1);
        begin = line.find(' ', begin + 1) + 1;

        g.push_back({ from, flow, {} });

        while (begin < line.size()) {
            std::string to = line.substr(begin, 2);
            g.back().neis.push_back(to);

            begin += 4;
        }
    }

    std::sort(g.begin(), g.end(), [](const node_t& l, const node_t& r) {
        if (l.flow > r.flow) {
            return true;
        }
        else if (l.flow == r.flow) {
            return l.name < r.name; /* AA should be right after valves with flow for cache size optimization (solution is without unordered_map). */
        }

        return false;
    });

    return g;
}


class graph_builder {
protected:
    std::unordered_map<std::string, int> name_to_id;

    std::vector<std::vector<int>> g;
    std::vector<int> cost;

    std::uint32_t VALVES_WITH_PRESSURE = 0;

public:
    graph_builder(graph_t& p_g) : g(p_g.size(), std::vector<int>(p_g.size(), 0)), cost(p_g.size(), -1) {
        int id_count = 0;

        for (const auto& node : p_g) {
            std::string name = node.name;

            auto iter = name_to_id.find(name);
            if (iter == name_to_id.cend()) {
                name_to_id[name] = id_count;

                id_count++;
            }

            if (node.flow > 0) {
                VALVES_WITH_PRESSURE++;
            }
        }

        for (const auto& node : p_g) {
            const int from_id = name_to_id[node.name];

            std::unordered_set<int> visited;
            std::queue<int> q;

            q.push(from_id);
            visited.insert(from_id);

            cost[from_id] = node.flow;
            int distance = 1;

            while (!q.empty()) {
                std::queue<int> q_next;

                while (!q.empty()) {
                    const int cur = q.front();
                    q.pop();

                    for (const auto& nei : p_g[cur].neis) {
                        const int to_id = name_to_id[nei];
                        if (visited.count(to_id)) {
                            continue;
                        }

                        visited.insert(to_id);

                        g[from_id][to_id] = distance;
                        g[to_id][from_id] = distance;

                        q_next.push(to_id);
                    }
                }

                q = std::move(q_next);
                distance++;
            }
        }
    }
};


class solution: public graph_builder {
    std::vector<std::uint32_t> cache;   /* using vector instead of unordered_map - only 16.252.928 elements are needed */

    std::uint32_t ALL_VALVES_OPEN = 0;
    std::uint32_t MAX_ID = 0;

    const std::uint32_t MAX_TIME = 31;

public:
    solution(graph_t& p_g) : graph_builder(p_g) {
        for (int i = 0; i < VALVES_WITH_PRESSURE; i++) {
            ALL_VALVES_OPEN <<= 1;
            ALL_VALVES_OPEN++;
        }

        auto count_bits_func = [](int value) -> int {
            int size_in_bits = 0;
            while (value > 0) {
                size_in_bits++;
                value >>= 1;
            }

            return size_in_bits;
        };

        const int MAX_STATE = std::uint32_t{ 1 } << VALVES_WITH_PRESSURE;
        MAX_ID = VALVES_WITH_PRESSURE + 1; /* All valves with pressure + initial position "AA" (whose ID goes right after valves with pressure) */
        cache = std::vector<std::uint32_t>(MAX_STATE * MAX_ID * MAX_TIME, -1);  /* cache size 16.252.928 */
    }

    int working_alone() {
        int id = name_to_id["AA"];
        return most_pressure_release(id, 30, 0);
    }

    int working_with_elephant() {
        int id = name_to_id["AA"];

        int best_pressure = 0;
        for (std::uint32_t state = 1; state < (ALL_VALVES_OPEN / 2); state++) {
            int pressure1 = most_pressure_release(id, 26, state);
            
            std::uint32_t opposite_state = (~state) & ALL_VALVES_OPEN;
            int pressure2 = most_pressure_release(id, 26, opposite_state);

            best_pressure = std::max(pressure1 + pressure2, best_pressure);
        }

        return best_pressure;
    }

private:
    int most_pressure_release(const int id, const int remaining_time, const std::uint32_t state) {
        if (remaining_time <= 0) {
            return 0;
        }

        int best_pressure = get_pressure_from_cache(id, remaining_time, state);
        if (best_pressure != -1) {
            return best_pressure;
        }

        best_pressure = 0;

        for (int i = 0; i < VALVES_WITH_PRESSURE; i++) {
            if (cost[i] == 0) {
                continue;   /* no need to open valve with 0 pressure */
            }

            const std::uint32_t mask = (std::uint32_t{ 1 } << i);
            if ((mask & state) != 0) {
                continue;   /* node is visited (max. number of nodes: 60) */
            }

            const int time_cost = g[id][i] + 1; /* time to reach and open */
            if (time_cost > remaining_time) {
                continue;
            }

            const std::uint32_t cur_state = state | mask;
            const int cur_remaining_time = remaining_time - time_cost;

            const int new_pressure = cost[i] * cur_remaining_time + most_pressure_release(i, cur_remaining_time, cur_state);

            best_pressure = std::max(best_pressure, new_pressure);
        }

        set_pressure_to_cache(id, remaining_time, state, best_pressure);
        return best_pressure;
    }

    int get_pressure_from_cache(const int id, const int remaining_time, const std::uint32_t state) {
        std::uint32_t key = (state * MAX_ID + id) * MAX_TIME + remaining_time;
        return cache[key];
    }

    void set_pressure_to_cache(const int id, const int remaining_time, const std::uint32_t state, const int pressure) {
        std::uint32_t key = (state * MAX_ID + id) * MAX_TIME + remaining_time;
        cache[key] = pressure;
    }
};


int main() {
    graph_t g = read_input();

    int total_pressure = solution(g).working_alone();
    std::cout << "The most pressure released (working alone): " << total_pressure << std::endl;

    total_pressure = solution(g).working_with_elephant();
    std::cout << "The most pressure released (working with elephant): " << total_pressure << std::endl;

    return 0;
}