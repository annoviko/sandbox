#include <vector>


class disjoint_sets {
private:
    std::vector<int> m_ids;
    std::vector<int> m_ranks;

public:
    disjoint_sets(const int p_nodes) :
        m_ids(p_nodes, 0),
        m_ranks(p_nodes, 1)
    {
        for (int i = 0; i < m_ids.size(); i++) {
            m_ids[i] = i;
        }
    }

public:
    int get_set_id(const int p_id) {
        if (p_id != m_ids[p_id]) {
            m_ids[p_id] = get_set_id(m_ids[p_id]);
        }

        return m_ids[p_id];
    }

    bool is_same_set(const int p_id1, const int p_id2) {
        const int set_id1 = get_set_id(p_id1);
        const int set_id2 = get_set_id(p_id2);

        return set_id1 == set_id2;
    }

    void merge(const int p_id1, const int p_id2) {
        const int set_id1 = get_set_id(p_id1);
        const int set_id2 = get_set_id(p_id2);

        if (m_ranks[set_id1] > m_ranks[set_id2]) {
            m_ids[set_id2] = set_id1;
            m_ranks[set_id1] += m_ranks[set_id2];
        }
        else {
            m_ids[set_id1] = set_id2;
            m_ranks[set_id2] += m_ranks[set_id1];
        }
    }
};