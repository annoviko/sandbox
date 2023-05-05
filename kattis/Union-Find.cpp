#include <vector>
#include <memory>


class disjoint_sets {
private:
    std::vector<int> m_ids;
    std::vector<int> m_rank;

public:
    disjoint_sets(const int n) :
        m_ids(n, 0),
        m_rank(n, 0)
    {
        for (int i = 0; i < n; i++) {
            m_ids[i] = i;
        }
    }

    int get_set_id(const int i) {
        if (m_ids[i] == i) {
            return i;
        }

        m_ids[i] = get_set_id(m_ids[i]);
        return m_ids[i];
    }

    void merge(const int i, const int j) {
        int id1 = get_set_id(i);
        int id2 = get_set_id(j);

        if (id1 == id2) {
            return;
        }

        if (m_rank[id1] >= m_rank[id2]) {
            m_ids[id2] = id1;
            m_rank[id1] += m_rank[id2];
        }
        else {
            m_ids[id1] = id2;
            m_rank[id2] += m_rank[id1];
        }
    }

    bool is_same_set(const int i, const int j) {
        return get_set_id(i) == get_set_id(j);
    }
};


#include <iostream>


int main() {
    /* 05-May-2023: The following three lines are needed for Kattis IO - otherwise 'time limit exceeded'. */
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n, q;
    std::cin >> n >> q;

    disjoint_sets sets(n);

    for (int i = 0; i < q; ++i) {
        char cmd;
        int a, b;

        std::cin >> cmd >> a >> b;

        if (cmd == '?') {
            std::cout << (sets.is_same_set(a, b) ? "yes" : "no") << '\n';
        }
        else {
            sets.merge(a, b);
        }
    }

    return 0;
}
