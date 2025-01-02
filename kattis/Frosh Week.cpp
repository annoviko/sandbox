#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> tasks(n, 0);
    std::vector<int> slots(m, 0);

    for (int i = 0; i < n; i++) {
        std::cin >> tasks[i];
    }

    for (int i = 0; i < m; i++) {
        std::cin >> slots[i];
    }

    std::sort(tasks.begin(), tasks.end());
    std::sort(slots.begin(), slots.end());

    int solved = 0;
    for (int i = 0, j = 0; i < tasks.size() && j < slots.size(); j++) {
        if (tasks[i] <= slots[j]) {
            solved++;
            i++;
        }
    }

    std::cout << solved << std::endl;
    return 0;
}