#include <iostream>
#include <fstream>
#include <regex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


struct node_t {
    int flow = 0;
    std::vector<std::string> neis;
};


using graph_t = std::unordered_map<std::string, node_t>;


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

        g[from].flow = flow;

        while (begin < line.size()) {
            std::string to = line.substr(begin, 2);
            g[from].neis.push_back(to);

            begin += 4;
        }
    }

    return g;
}


class working {
protected:
    std::unordered_map<std::string, int> name_to_id;
    std::unordered_map<int, std::string> id_to_name;

    std::vector<std::vector<int>> g;
    std::vector<int> cost;

    std::uint64_t ALL_VISITED = 0;

public:
    working(graph_t& p_g) : g(p_g.size(), std::vector<int>(p_g.size(), 0)), cost(p_g.size(), -1) {
        int id_count = 0;

        for (const auto& pair : p_g) {
            std::string name = pair.first;

            auto iter = name_to_id.find(name);
            if (iter == name_to_id.cend()) {
                id_to_name[id_count] = name;
                name_to_id[name] = id_count;

                id_count++;
            }
        }

        for (const auto& pair : p_g) {
            const int from_id = name_to_id[pair.first];

            std::unordered_set<int> visited;
            std::queue<int> q;

            q.push(from_id);
            visited.insert(from_id);

            cost[from_id] = pair.second.flow;
            int distance = 1;

            while (!q.empty()) {
                std::queue<int> q_next;

                while (!q.empty()) {
                    const int cur = q.front();
                    q.pop();

                    std::string cur_name = id_to_name[cur];

                    for (const auto& nei : p_g[cur_name].neis) {
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

        for (int i = 0; i < g.size(); i++) {
            ALL_VISITED <<= 1;
            ALL_VISITED++;
        }
    }
};


class working_alone : public working {
    struct cache_key_t {
        int id = -1;
        int remaining_time;
        std::uint64_t state;

        bool operator==(const cache_key_t& p_other) const {
            return (id == p_other.id) && (remaining_time == p_other.remaining_time) && (state == p_other.state);
        }
    };


    struct cache_key_hash {
        std::size_t operator()(const cache_key_t& key) const noexcept {
            std::size_t h = std::hash<std::uint64_t>{}(key.state);

            h ^= std::hash<int>{}(key.id) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<int>{}(key.remaining_time) + 0x9e3779b9 + (h << 6) + (h >> 2);

            return h;
        }
    };

    std::unordered_map<cache_key_t, int, cache_key_hash> cache;

public:
    working_alone(graph_t& p_g) : working(p_g) { }

    int most_pressure_release() {
        int id = name_to_id["AA"];
        return most_pressure_release(id, 30, 0);
    }

private:
    int most_pressure_release(const int id, const int remaining_time, const std::uint64_t state) {
        if (remaining_time <= 0) {
            return 0;
        }

        if (state == ALL_VISITED) {
            return 0;
        }

        int best_pressure = get_pressure_from_cache(id, remaining_time, state);
        if (best_pressure != -1) {
            return best_pressure;
        }

        best_pressure = 0;

        for (int i = 0; i < g.size(); i++) {
            if (cost[i] == 0) {
                continue;   /* no need to open valve with 0 pressure */
            }

            const std::uint64_t mask = (std::uint64_t{ 1 } << i);
            if ((mask & state) != 0) {
                continue;   /* node is visited (max. number of nodes: 60) */
            }

            const int time_cost = g[id][i] + 1; /* time to reach and open */

            const std::uint64_t cur_state = state | mask;
            const int cur_remaining_time = remaining_time - time_cost;

            const int new_pressure = cost[i] * cur_remaining_time + most_pressure_release(i, cur_remaining_time, cur_state);

            best_pressure = std::max(best_pressure, new_pressure);
        }

        set_pressure_to_cache(id, remaining_time, state, best_pressure);
        return best_pressure;
    }

    int get_pressure_from_cache(const int id, const int remaining_time, const std::uint64_t state) {
        const cache_key_t key{ id, remaining_time, state };
        auto iter = cache.find(key);
        if (iter == cache.cend()) {
            return -1;
        }

        return iter->second;
    }

    void set_pressure_to_cache(const int id, const int remaining_time, const std::uint64_t state, const int pressure) {
        const cache_key_t key{ id, remaining_time, state };
        cache[key] = pressure;
    }
};


int main() {
    graph_t g = read_input();

    int total_pressure = working_alone(g).most_pressure_release();
    std::cout << "The most pressure which can be released: " << total_pressure << std::endl;

    return 0;
}