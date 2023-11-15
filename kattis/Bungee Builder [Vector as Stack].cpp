#include <iostream>
#include <vector>
#include <stack>




int get_max_height(const std::vector<int>& in) {
    std::vector<int> s(in.size());  /* std::stack is OK, and accepted by Kattis (Accepted(100)), but vector just faster */
    int index_top = -1;

    int global_max = 0;

    for (int i = 0; i < in.size(); i++) {
        if (index_top < 0) {
            s[++index_top] = in[i];
            continue;
        }

        if (in[i] < s[index_top]) {
            s[++index_top] = in[i];
            continue;
        }

        int min_node = in[i];
        int max_height = 0;
        while (index_top >= 0 && in[i] > s[index_top]) {
            min_node = std::min(min_node, s[index_top]);
            int max_height_candidate = std::min(s[index_top], in[i]) - min_node;
            max_height = std::max(max_height_candidate, max_height);

            index_top--;
        }

        if (index_top >= 0) {
            min_node = std::min(min_node, s[index_top]);
            int max_height_candidate = std::min(s[index_top], in[i]) - min_node;
            max_height = std::max(max_height_candidate, max_height);
        }

        if (max_height > 0) {
            if (!s.empty()) {
                s[++index_top] = min_node;
            }
        }

        s[++index_top] = in[i];
        global_max = std::max(max_height, global_max);
    }

    return global_max;
}


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> heights(n);
    for (int i = 0; i < n; i++) {
        std::cin >> heights[i];
    }

    std::cout << get_max_height(heights) << std::endl;
    return 0;
}