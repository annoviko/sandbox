#include <algorithm>
#include <vector>
#include <iostream>


struct interval {
    double begin;
    double end;
    int index = -1;
};


int main() {
    double interval_begin = 0;
    while (std::cin >> interval_begin) {
        interval ab;
        ab.begin = interval_begin;
        std::cin >> ab.end;

        int n = 0;
        std::cin >> n;

        std::vector<interval> fractions(n);
        for (int i = 0; i < n; i++) {
            std::cin >> fractions[i].begin >> fractions[i].end;
            fractions[i].index = i;
        }

        std::sort(fractions.begin(), fractions.end(), [](const interval& l, const interval& r) {
            if (l.begin == r.begin) {
                return l.end > r.end;
            }

            return l.begin < r.begin;
        });

        std::vector<interval> to_cover;
        double cur_end = ab.begin;
        int i = 0;

        while (cur_end < ab.end || to_cover.empty()) {
            int best_index = -1;
            double best_next_end = cur_end;

            while (i < fractions.size() && fractions[i].begin <= cur_end) {
                if (best_next_end <= fractions[i].end) {
                    best_next_end = fractions[i].end;
                    best_index = i;
                }

                i++;
            }

            if (best_index == -1) {
                to_cover.clear();
                break;
            }

            to_cover.push_back(fractions[best_index]);
            cur_end = best_next_end;
        }

        if (to_cover.empty()) {
            std::cout << "impossible" << std::endl;
        }
        else {
            std::cout << to_cover.size() << std::endl;
            for (int i = 0; i < to_cover.size(); i++) {
                if (i != 0) {
                    std::cout << ' ';
                }

                std::cout << to_cover[i].index;
            }
            std::cout << std::endl;
        }
    }

    return 0;
}