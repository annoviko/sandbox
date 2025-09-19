#include <vector>
#include <algorithm>
#include <numeric>

void backwards(std::vector<int>& vec) {
    std::reverse(vec.begin(), vec.end());
}

std::vector<int> everyOther(const std::vector<int>& vec) {
    std::vector<int> r;
    for (int i = 0; i < vec.size(); i += 2) {
        r.push_back(vec[i]);
    }
    return r;
}

int smallest(const std::vector<int>& vec) {
    return *(std::min_element(vec.begin(), vec.end()));
}

int sum(const std::vector<int>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

int veryOdd(const std::vector<int>& vec) {
    int counter = 0;
    for (int i = 1; i < vec.size(); i += 2) {
        if (vec[i] % 2 != 0) {
            counter++;
        }
    }

    return counter;
}