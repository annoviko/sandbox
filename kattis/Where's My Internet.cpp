#include <iostream>
#include <vector>


class disjoint_set {
private:
    std::vector<int> m_ids;
    std::vector<int> m_rank;

public:
    disjoint_set(const std::size_t p_len) :
        m_ids(p_len),
        m_rank(p_len, 1)
    { 
        for (int i = 0; i < p_len; i++) {
            m_ids[i] = i;
        }
    }

public:
    int get_set_id(const int node_id) {
        if (node_id != m_ids[node_id]) {
            m_ids[node_id] = get_set_id(m_ids[node_id]);
        }

        return m_ids[node_id];
    }


    void merge(const int id1, const int id2) {
        int set_id1 = get_set_id(id1);
        int set_id2 = get_set_id(id2);

        if (set_id1 == set_id2) { return; }

        if (m_rank[set_id1] > m_rank[set_id2]) {
            m_rank[set_id1] += m_rank[set_id2];
            m_ids[set_id2] = set_id1;
        }
        else {
            m_rank[set_id2] += m_rank[set_id1];
            m_ids[set_id1] = set_id2;
        }
    }


    std::vector<int> get_non_merged_to(const int node_id1) {
        int set_id1 = get_set_id(node_id1);

        std::vector<int> nodes;
        for (int i = 0; i < m_ids.size(); i++) {
            int set_id2 = get_set_id(i);
            if (set_id2 != set_id1) {
                nodes.push_back(i);
            }
        }

        return nodes;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    disjoint_set ds(n);
    for (int i = 0; i < m; i++) {
        int from, to;
        std::cin >> from >> to;

        ds.merge(from - 1, to - 1);
    }

    std::vector<int> non_connected = ds.get_non_merged_to(0);
    if (non_connected.empty()) {
        std::cout << "Connected" << std::endl;
    }
    else {
        for (auto node_id : non_connected) {
            std::cout << node_id + 1 << std::endl;
        }
    }

    return 0;
}