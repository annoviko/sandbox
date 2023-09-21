#include <iostream>
#include <vector>


class bit_tree {
private:
    std::vector<int> m_tree;
    std::vector<int> m_vals_to_idx;
    int m_offset = 0;

    int m_begin = 0;
    int m_end = 0;

public:
    bit_tree(const std::vector<int>& p_array) :
        m_tree(p_array.size() * 3 + 1),
        m_vals_to_idx(p_array.size() + 1, 0),
        m_offset(p_array.size()),
        m_begin(1),
        m_end(m_tree.size() - 1)
    {
        for (int i = 0; i < p_array.size(); i++) {
            update(i + m_offset, 1);
            m_vals_to_idx[p_array[i]] = i + m_offset;
        }
    }

private:
    void update(const int index, const int value) {
        for (int i = index; i < m_tree.size(); i += (i & (-i))) {
            m_tree[i] += value;
        }
    }

    int count(const int to) {
        int sum = 0;
        for (int i = to; i > 0; i -= (i & (-i))) {
            sum += m_tree[i];
        }

        return sum;
    }

    int count(const int from, const int to) {
        return count(to - 1) - count(from);
    }

public:
    int move_element_to_begin(const int value) {
        int index_in_tree = m_vals_to_idx[value];
        int swaps = count(m_begin, index_in_tree);

        update(index_in_tree, -1);
        update(m_begin, 1);

        m_begin++;

        return swaps;
    }

    int move_element_to_end(const int value) {
        int index_in_tree = m_vals_to_idx[value];
        int swaps = count(index_in_tree, m_end);

        update(index_in_tree, -1);
        update(m_end, 1);

        m_end--;

        return swaps;
    }
};


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    bit_tree tree(arr);

    int begin = 1;
    int end = arr.size();
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            std::cout << tree.move_element_to_begin(begin) << std::endl;
            begin++;
        }
        else {
            std::cout << tree.move_element_to_end(end) << std::endl;
            end--;
        }
    }

    return 0;
}