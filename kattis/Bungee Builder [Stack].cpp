#include <iostream>
#include <vector>
#include <stack>




int get_max_height(const std::vector<int>& in) {
    std::stack<int> s;
    int global_max = 0;

    for (int i = 0; i < in.size(); i++) {
        if (s.empty()) {
            s.push(in[i]);
            continue;
        }

        if (in[i] < s.top()) {
            s.push(in[i]);
            continue;
        }

        int min_node = in[i];
        int max_height = 0;
        while (!s.empty() && in[i] > s.top()) {
            min_node = std::min(min_node, s.top());
            int max_height_candidate = std::min(s.top(), in[i]) - min_node;
            max_height = std::max(max_height_candidate, max_height);

            s.pop();
        }

        if (!s.empty()) {
            min_node = std::min(min_node, s.top());
            int max_height_candidate = std::min(s.top(), in[i]) - min_node;
            max_height = std::max(max_height_candidate, max_height);
        }

        if (max_height > 0) {
            if (!s.empty()) {
                s.push(min_node);
            }
        }

        s.push(in[i]);
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