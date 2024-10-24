#include <algorithm>
#include <queue>
#include <iostream>
#include <vector>


struct moose {
    int year;
    long long strength;
};


struct moose_comparator {
    bool operator()(const moose& l, const moose& r) {
        return l.strength < r.strength;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);

    int k, n;
    std::cin >> k >> n;

    moose karl;
    std::cin >> karl.year >> karl.strength;

    std::vector<moose> tribe = { karl };
    for (int i = 0; i < (n + k - 2); i++) {
        moose participant;
        std::cin >> participant.year >> participant.strength;

        tribe.push_back(participant);
    }

    std::sort(tribe.begin(), tribe.end(), [](const moose& l, const moose& r) {
        return l.year < r.year;
    });

    std::priority_queue<moose, std::vector<moose>, moose_comparator> queue;
    for (int i = 0; i < k; i++) {
        queue.push(tribe[i]);
    }

    int current_year = tribe[0].year;
    if (queue.top().strength == karl.strength) {
        std::cout << current_year << std::endl;
        return 0;
    }

    queue.pop();

    for (int index_moose = k; index_moose < tribe.size(); index_moose++) {
        current_year++;

        queue.push(tribe[index_moose]);
        if (queue.top().strength == karl.strength) {
            std::cout << current_year << std::endl;
            return 0;
        }

        queue.pop();
    }

    std::cout << "unknown" << std::endl;
    return 0;
}