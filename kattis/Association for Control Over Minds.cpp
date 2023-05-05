#include <vector>
#include <unordered_map>


class disjoint_sets {
private:
    std::vector<int> m_ids;
    std::vector<int> m_rank;

public:
    disjoint_sets(int n) : m_ids(n, 0), m_rank(n, 1){
        for (int i = 0; i < n; i++) {
            m_ids[i] = i;
        }
    }


public:
    int get_set_id(const int i) {
        if (m_ids[i] != i) {
            m_ids[i] = get_set_id(m_ids[i]);
        }

        return m_ids[i];
    }


    bool is_same_set(const int i, const int j) {
        return get_set_id(i) == get_set_id(j);
    }


    int get_set_size(const int i) {
        return m_rank[get_set_id(i)];
    }


    void merge(const int i, const int j) {
        int id1 = get_set_id(i);
        int id2 = get_set_id(j);

        if (id1 == id2) {
            return;
        }

        if (m_ids[id1] > m_ids[id2]) {
            m_ids[id2] = id1;
            m_rank[id1] += m_rank[id2];
        }
        else {
            m_ids[id1] = id2;
            m_rank[id2] += m_rank[id1];
        }
    }


    bool try_merge(const std::vector<int>& p_elems) {
        std::unordered_map<int, int> union_sizes;

        for (int elem : p_elems) {
            int id = get_set_id(elem);
            union_sizes[id]++;
        }

        for (const auto& pair : union_sizes) {
            int id = pair.first;
            if (union_sizes[id] != m_rank[id]) {
                return false; /* cannot merge since we cannot reuse completelly */
            }
        }

        /* merge */
        int anchor_id = union_sizes.begin()->first;
        for (const auto& pair : union_sizes) {
            merge(anchor_id, pair.first);
        }

        return true;
    }
};


#include <iostream>

int main() {
    int n;
    std::cin >> n;

    disjoint_sets sets(500001);

    int counter = 0;

    for (int i = 0; i < n; i++) {
        int m;
        std::cin >> m;

        std::vector<int> elems(m, 0);
        for (int j = 0; j < m; j++) {
            std::cin >> elems[j];
        }

        if (sets.try_merge(elems)) {
            counter++;
        }
    }

    std::cout << counter << std::endl;
    return 0;
}