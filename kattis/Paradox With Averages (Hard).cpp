#include <iostream>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::vector<long long int> cs;
        std::vector<long long int> ec;

        int ncs, nec;
        std::cin >> ncs >> nec;

        for (int j = 0; j < ncs; j++) {
            int score;
            std::cin >> score;

            cs.push_back(score);
        }

        for (int j = 0; j < nec; j++) {
            int score;
            std::cin >> score;

            ec.push_back(score);
        }

        std::int64_t sum_cs = 0;
        for (auto v : cs) {
            sum_cs += v;
        }

        std::int64_t sum_ec = 0;
        for (auto v : ec) {
            sum_ec += v;
        }

        long double avg_cs = (long double)sum_cs / (long double)ncs;
        long double avg_ec = (long double)sum_ec / (long double)nec;

        int result = 0;
        for (int j = 0; j < cs.size(); j++) {
            if (cs[j] < avg_cs && cs[j] > avg_ec) {
                result++;
            }
        }

        std::cout << result << std::endl;
    }

    return 0;
}