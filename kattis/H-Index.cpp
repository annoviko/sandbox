#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n;
    std::cin >> n;

    std::vector<int> stats(n);
    for (int i = 0; i < n; i++) {
        std::cin >> stats[i];
    }

    std::sort(stats.begin(), stats.end());  /* O(N*log(N)) */

    int hindex = -1;
    for (int i = 0; i < stats.size(); i++) {    /* O(N) */
        int cite_index = stats[i];
        int papers_with_higher_index = stats.size() - i;

        int hindex_candidate = std::min(cite_index, papers_with_higher_index);
        hindex = std::max(hindex, hindex_candidate);
    }

    std::cout << hindex << std::endl;   /* Final: O(N * log(N)) */

    return 0;
}
