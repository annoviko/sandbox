#include <iostream>
#include <vector>


class fenwick_tree {
private:
    std::vector<std::int64_t> m_tree;

public:
    fenwick_tree(const std::int64_t m_size) {
        m_tree.assign(m_size + 1, 0);
    }


public:
    void add(std::int64_t p_index, std::int64_t value) {
        p_index++;

        while (p_index < m_tree.size()) {
            m_tree[p_index] += value;
            p_index += (p_index & -p_index);
        }
    }

    std::int64_t sum(std::int64_t p_index) {
        std::int64_t result = 0;
        while (p_index > 0) {
            result += m_tree[p_index];
            p_index -= (p_index & -p_index);
        }

        return result;
    }
};


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, q;
    std::cin >> n >> q;

    fenwick_tree tree(n);

    for (int i = 0; i < q; i++) {
        char operation = 0;
        std::cin >> operation;

        if (operation == '?') {
            std::int64_t index = 0;
            std::cin >> index;

            std::cout << tree.sum(index) << '\n';   // `\n` is faster than `std::cin` - without it expect time limit exceeded.
        }
        else if (operation == '+') {
            std::int64_t index = 0, value = 0;

            std::cin >> index >> value;
            tree.add(index, value);
        }
    }

    return 0;
}
