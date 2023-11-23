#include <iostream>
#include <vector>


class fenwick_tree {
    std::vector<int> m_tree;

public:
    fenwick_tree() = default;

    fenwick_tree(const int h) : m_tree(h + 1) { }

public:
    void increment(int from) {
        for (int i = from; i < m_tree.size(); i += (i & (-i))) {
            m_tree[i]++;
        }
    }

    int count(const int to) {   /* log(H) */
        int sum = 0;
        for (int i = to; i > 0; i -= (i & (-i))) {
            sum += m_tree[i];
        }

        return sum;
    }

    int count(const int from, const int to) {   /* 2 * log(H) */
        return count(to) - count(from - 1);
    }

    int size() const { return m_tree.size(); }
};


class solution {
private:
    fenwick_tree m_tree_up;
    fenwick_tree m_tree_down;
    int height = 0;

public:
    solution(const int h, const std::vector<int>& p_content) : height(h) {  /* O(N * log2(H)) */
        int up_max = 0;
        int down_max = 0;

        for (int i = 0; i < p_content.size(); i++) {    /* O(N) */
            if (i % 2 == 0) {
                up_max = std::max(p_content[i], up_max);
            }
            else {
                down_max = std::max(p_content[i], down_max);
            }
        }

        m_tree_up = fenwick_tree(up_max);
        m_tree_down = fenwick_tree(down_max);

        for (int i = 0; i < p_content.size(); i++) {    /* O(N * log2(H)) */
            if (i % 2 == 0) {
                m_tree_up.increment(up_max - p_content[i] + 1);
            }
            else {
                m_tree_down.increment(down_max - p_content[i] + 1);
            }
        }
    }

public:
    std::pair<int, int> find_minimum(const std::vector<int>& p_content) {   /* N * log2(H) */
        int minimum_value = 2147483647;
        int frequency = 0;
        for (int i = 0; i < height; i++) {   /* N * log2(H) */
            int index_up = m_tree_up.size() - i - 1;
            int i_down = height - i;

            int obstacles = 0;
            if (index_up < m_tree_up.size()) {
                obstacles += m_tree_up.count(index_up);
            }

            if (i_down < m_tree_down.size()) {
                int index_down = m_tree_down.size() - i_down;
                if (index_down < m_tree_down.size()) {
                    obstacles += m_tree_down.count(index_down);
                }
            }

            //std::cout << "Level #" << i << ": " << obstacles << std::endl;

            if (obstacles < minimum_value) {
                minimum_value = obstacles;
                frequency = 1;
            }
            else if (obstacles == minimum_value) {
                frequency++;
            }
        }

        return { minimum_value, frequency };
    }
};



int main() {
    int n, h;
    std::cin >> n >> h;

    std::vector<int> pikes(n);
    for (int i = 0; i < pikes.size(); i++) {
        std::cin >> pikes[i];
    }

    auto result = solution(h, pikes).find_minimum(pikes);       /* N * log2(H) */
    std::cout << result.first << ' ' << result.second;

    return 0;
}