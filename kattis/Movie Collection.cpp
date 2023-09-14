#include <iostream>
#include <numeric>
#include <vector>


class fenwick_tree_for_movies {
private:
    std::int64_t m_basic_offset = 0;

    std::vector<std::int64_t> m_tree;
    std::vector<std::int64_t> m_index_to_position;
    std::int64_t m_free_index = 0;


public:
    fenwick_tree_for_movies(const std::int64_t p_array_size, const std::int64_t p_queue_size) :
        m_basic_offset(p_queue_size + 1),
        m_tree(p_array_size + m_basic_offset, 0),
        m_index_to_position(p_array_size + 1, 0),
        m_free_index(m_basic_offset - 1)
    {
        for (int i = 0; i < p_array_size; i++) {    // O(N)
            const int index = i + m_basic_offset;

            m_tree[index]++;

            const int index_parent = index + (index & (-index));
            if (index_parent < m_tree.size()) {
                m_tree[index_parent] += m_tree[index];
            }

            m_index_to_position[i + 1] = i + m_basic_offset;
        }
    }

public:
    std::int64_t watch_movie(const std::int64_t p_movie_index) {    /* O(log(N)) */
        const auto tree_index = m_index_to_position[p_movie_index];
        if (tree_index == m_free_index + 1) {
            return 0;   /* the movie is on the top */
        }

        int64_t movies_before = count_movies_before(tree_index) - 1; /* do not count itself */  /* O(log(N)) */

        update(tree_index, -1);    /* remove movie from its current position */ /* O(log(N)) */
        update(m_free_index, 1);   /* place movie in front */   /* O(log(N)) */

        m_index_to_position[p_movie_index] = m_free_index;
        m_free_index--;

        return movies_before;
    }


    void update(const std::int64_t p_index, const int64_t p_value) {
        for (std::int64_t i = p_index; i < m_tree.size(); i += (i & -i)) {
            m_tree[i] += p_value;
        }
    }


    std::int64_t count_movies_before(std::int64_t p_index) {
        std::int64_t sum = 0;
        for (std::int64_t i = p_index; i > 0 /*m_free_index + m_basic_offset*/; i -= (i & -i)) {
            sum += m_tree[i];
        }

        return sum;
    }
};



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int m, r;
        std::cin >> m >> r;

        fenwick_tree_for_movies tree(m, r);

        for (int k = 0; k < r; k++) {   /* O(R * log(M)) */
            int movie_id;
            std::cin >> movie_id;

            if (k != 0) {
                std::cout << ' ';
            }

            std::cout << tree.watch_movie(movie_id);
        }

        std::cout << std::endl;
    }

    return 0;
}
