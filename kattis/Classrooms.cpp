#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>


struct entry {
    std::int64_t begin = 0;
    std::int64_t end = 0;
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<entry> entries(n);
    for (int i = 0; i < n; i++) {
        std::cin >> entries[i].begin >> entries[i].end;
    }

    std::sort(entries.begin(), entries.end(), [](const entry& l, const entry& r) {
        if (l.end == r.end) {
            return l.begin < r.begin;
        }

        return l.end < r.end;
        });

    std::multiset<std::int64_t> endtime;
    for (int i = 0; i < m; i++) {
        endtime.insert(0);
    }

    int scheduled = 0;
    for (const entry& e : entries) {
        auto iter = endtime.lower_bound(e.begin);
        if (iter != endtime.begin()) {
            iter--;
            endtime.erase(iter);
            endtime.insert(e.end);
            scheduled++;
        }
    }

    std::cout << scheduled << std::endl;

    return 0;
}