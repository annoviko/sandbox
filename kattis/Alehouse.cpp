#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>


struct time_entry {
    int in;
    int out;
};


struct time_entry_comparator {
    bool operator()(const time_entry& l, const time_entry& r) {
        return l.out > r.out;
    }
};


int main() {
    int n, k;
    std::cin >> n >> k;

    std::vector<time_entry> entries(n);
    for (int i = 0; i < n; i++) {
        std::cin >> entries[i].in >> entries[i].out;
    }

    std::sort(entries.begin(), entries.end(), [](const time_entry& l, const time_entry& r) {
        return l.in < r.in;
    });

    std::priority_queue<time_entry, std::vector<time_entry>, time_entry_comparator> queue;
    std::size_t max_friends = 0;

    for (int i = 0; i < n; i++) {
        time_entry to_add = entries[i];
        while (!queue.empty() && (to_add.in - queue.top().out > k)) {
            queue.pop();
        }

        queue.push(to_add);
        max_friends = std::max(queue.size(), max_friends);
    }

    std::cout << max_friends << std::endl;
    return 0;
}
