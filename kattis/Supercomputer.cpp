#include <iostream>
#include <vector>


#if 0
#define TEST_SOLUTION
#endif


class fenwick_tree {
private:
    std::vector<std::int64_t> m_tree;
    std::vector<std::int64_t> m_values;

public:
    fenwick_tree(const std::int64_t p_size) :
        m_tree(p_size + 1, 0),
        m_values(p_size + 1, 0)
    { }

public:
    void flip(std::int64_t p_index) {
        std::int64_t value = 0;
        if (m_values[p_index] == 0) {
            value = 1;
            m_values[p_index] = 1;
        }
        else {
            value = -1;
            m_values[p_index] = 0;
        }

        for (std::int64_t i = p_index; i < m_tree.size(); i += (i & -i)) {
            m_tree[i] += value;
        }
    }

    std::int64_t count_ones(std::int64_t p_index) {
        std::int64_t sum = 0;
        for (std::int64_t i = p_index; i > 0; i -= (i & -i)) {
            sum += m_tree[i];
        }

        return sum;
    }

    std::int64_t count_ones(std::int64_t p_from, std::int64_t p_to) {
        return count_ones(p_to) - count_ones(p_from - 1);
    }

#if defined(TEST_SOLUTION)
    std::int64_t verify_count_ones(std::int64_t p_index) {
        std::int64_t sum = 0;
        for (std::int64_t i = 0; i <= p_index; i++) {
            if (m_values[i] == 1) {
                sum++;
            }
        }

        return sum;
    }

    std::int64_t verify_count_ones(std::int64_t p_from, std::int64_t p_to) {
        std::int64_t sum = 0;
        for (std::int64_t i = p_from; i <= p_to; i++) {
            if (m_values[i] == 1) {
                sum++;
            }
        }

        return sum;
    }
#endif
};


#if defined(TEST_SOLUTION)
#include <cstdlib>

void test() {
    int n = 100;
    fenwick_tree tree(n);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 100000; i++) {
        int index = std::rand() % n + 1;
        tree.flip(index);

        if (tree.count_ones(index) != tree.verify_count_ones(index)) {
            std::cout << "FAIL" << std::endl;
        }

        int from = std::rand() % (n / 2) + 1;
        int to = std::rand() % (n / 2) + n / 2;
        if (tree.count_ones(from, to) != tree.verify_count_ones(from, to)) {
            std::cout << "FAIL" << std::endl;
        }

        index = std::rand() % n + 1;
        if (tree.count_ones(index) != tree.verify_count_ones(index)) {
            std::cout << "FAIL" << std::endl;
        }
    }
}
#endif


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

#if defined(TEST_SOLUTION)
    test();
#endif

    std::int64_t n, k;
    std::cin >> n >> k;

    fenwick_tree tree(n);

    for (std::int64_t i = 0; i < k; i++) {
        char operation = 0;
        std::cin >> operation;

        if (operation == 'F') {
            std::int64_t index;
            std::cin >> index;

            tree.flip(index);
        }
        else {
            std::int64_t from, to;
            std::cin >> from >> to;

            std::cout << tree.count_ones(from, to) << '\n';
        }
    }

    return 0;
}