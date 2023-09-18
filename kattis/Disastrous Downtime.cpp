#include <cmath>
#include <iostream>
#include <vector>


class fenwick_tree_downtime {
private:
    std::vector<int> m_tree;
    int m_max_req = 0;

public:
    fenwick_tree_downtime() :
        m_tree(100002, 0)
    { }

private:
    int count(const int p_from, const int p_to) {
        return count(p_to) - count(p_from - 1);
    }


    int count(const int p_to) {
        int result = 0;
        for (int i = p_to; i > 0; i -= (i & (-i))) {
            result += m_tree[i];
        }

        return result;
    }

public:
    void add_request(int p_time) {
        p_time++;
        for (int i = p_time; i < m_tree.size(); i += (i & (-i))) {
            m_tree[i]++;
        }

        /* get requests for previous 999 */
        int loading = count(p_time - 999, p_time);
        m_max_req = std::max(loading, m_max_req);
    }

    int get_max_loading() {
        return m_max_req;
    }
};


int main() {
    int n, k;
    std::cin >> n >> k;

    fenwick_tree_downtime tree;
    for (int i = 0; i < n; i++) {
        int t;
        std::cin >> t;

        tree.add_request(t);
    }

    std::cout << std::ceil(static_cast<double>(tree.get_max_loading()) / static_cast<double>(k)) << std::endl;
    return 0;
}