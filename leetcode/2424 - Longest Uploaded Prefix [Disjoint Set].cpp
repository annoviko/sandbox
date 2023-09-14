#include <numeric>
#include <vector>


class disjoint_sets {
private:
    std::vector<int> sets;
    std::vector<int> set_sizes;

public:
    disjoint_sets(int n) :
        sets(n),
        set_sizes(n, 1)
    {
        std::iota(sets.begin(), sets.end(), 0);
    }

public:
    int get_set_id(int id) {    /* amortized O(1) */
        if (id != sets[id]) {
            sets[id] = get_set_id(sets[id]);
        }

        return sets[id];
    }

    int get_set_size(int id) {  /* O(1) */
        const int set_id = get_set_id(id);
        return set_sizes[set_id];
    }

    void merge(int id1, int id2) {  /* O(1) */
        int set_id1 = get_set_id(id1);
        int set_id2 = get_set_id(id2);

        if (set_id1 == set_id2) {
            return;
        }

        if (set_id1 < set_id2) {
            sets[set_id2] = set_id1;
            set_sizes[set_id1] += set_sizes[set_id2];
        }
        else {
            sets[set_id1] = set_id2;
            set_sizes[set_id2] += set_sizes[set_id1];
        }
    }
};


class LUPrefix {
private:
    disjoint_sets m_sets;
    std::vector<bool> m_map;

public:
    LUPrefix(int n) : 
        m_sets(n + 1),
        m_map(n + 1, false)
    { }

    void upload(int video) {
        m_map[video] = true;
        if (video > 0 && m_map[video - 1] == true) {
            m_sets.merge(video, video - 1);
        }

        if (video + 1 < m_map.size() && m_map[video + 1] == true) {
            m_sets.merge(video, video + 1);
        }
    }

    int longest() {
        if (m_map[1] == false) {
            return 0;
        }

        const int set_size = m_sets.get_set_size(1);
        return set_size;
    }
};
