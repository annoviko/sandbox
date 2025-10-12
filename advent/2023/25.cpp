#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <unordered_set>


using graph = std::vector<std::vector<int>>;


struct weighted_edge_t {
    int to;
    int w;
};


class solution {
private:
    graph m_g;
    std::vector<int> m_sz;

public:
    solution(const graph& p_g) :
        m_g(p_g),
        m_sz(p_g.size(), 1)
    { }

public:
    int disconnect_three_wires() {
        std::unordered_map<int, std::unordered_map<int, int>> g;
        for (int from = 0; from < m_g.size(); from++) {
            for (int to : m_g[from]) {
                g[from].insert({ to, 1 });
            }
        }

        return minimum_cut(g, 3);
    }

private:
    int minimum_cut(std::unordered_map<int, std::unordered_map<int, int>>& g, const int edges_to_cut) {
        while (g.size() > 1) {
            const int subgraph_size = minimum_cut_phase(g, edges_to_cut);
            if (subgraph_size != -1) {
                const int other_size = m_g.size() - subgraph_size;
                return subgraph_size * other_size;
            }
        }

        throw std::exception((std::string("impossible to cut graph by using '") + std::to_string(edges_to_cut) + std::string("' edges.")).c_str());
    }

    int minimum_cut_phase(std::unordered_map<int, std::unordered_map<int, int>>& g, int edges_to_cut) {
        std::unordered_set<int> set;
        std::vector<int> weights(m_g.size(), 0);

        int cur = g.begin()->first;
        set.insert(cur);

        int s = -1;
        int t = cur;

        while (g.size() != set.size()) {
            for (const auto& [id, w] : g[cur]) {
                if (set.count(id) == 0) {
                    weights[id] += w;
                }
            }

            /* most tightly connected vertex  */
            int node = -1;
            int connectivity = 0;

            for (const auto& [id, neis] : g) {
                if (set.count(id) > 0) {
                    continue;
                }

                if (connectivity < weights[id]) {
                    node = id;
                    connectivity = weights[id];
                }
            }

            if (node == -1) {
                throw std::exception("no connected nodes to the set");
            }

            /* add it to the set */
            set.insert(node);

            s = t;
            t = node;
            cur = node;
        }

        if (s == -1 || t == -1) {
            std::cout << std::endl;
        }

        /* cut of phase value */
        int cut_of_the_phase_value = 0;
        for (const auto& [nei, w] : g[t]) {
            if (set.count(nei) == 0) {
                continue;
            }

            cut_of_the_phase_value += w;
        }

        if (cut_of_the_phase_value == edges_to_cut) {
            return m_sz[t];
        }

        /* merge s and t */
        for (const auto& [nei, w] : g[t]) {
            if (nei == s) {
                continue;
            }

            /* update s */
            g[s][nei] += w;
            g[nei][s] += w;

            g[nei].erase(t);
        }

        m_sz[s] += m_sz[t];
        m_sz[t] = 0;

        g[s].erase(t);
        g.erase(t);

        return -1;
    }
};


struct graph_view_t {
    graph g;
    std::unordered_map<std::string, int> names;
    std::vector<std::string> ids;

public:
    int get_id(const std::string& node) {
        if (names.count(node) == 0) {
            names[node] = g.size();
            ids.push_back(node);
            g.push_back({});
        }

        return names[node];
    }
};


graph_view_t read_input() {
    std::fstream file("input.txt");

    graph_view_t view;

    for (std::string line; std::getline(file, line);) {
        std::stringstream ss(line);

        std::string from;
        ss >> from;
        from.pop_back();

        int from_id = view.get_id(from);

        for (std::string to; ss >> to;) {
            int to_id = view.get_id(to);

            view.g[from_id].push_back(to_id);
            view.g[to_id].push_back(from_id);
        }
    }

    return view;
}


int main() {
    const auto graph_view = read_input();

    int split_value = solution(graph_view.g).disconnect_three_wires();
    std::cout << "Multiplication of two groups together: " << split_value << std::endl;

    return 0;
}