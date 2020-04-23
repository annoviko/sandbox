#include <iostream>
#include <vector>

using std::vector;

vector<int> optimal_summands(int n) {
    vector<int> prizes;

    int substructor = 1;
    while (n > 0 && substructor <= n) {
        prizes.push_back(substructor);
        n -= substructor++;
    }

    if (n > 0) {
        prizes.back() += n;
    }

    return prizes;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> summands = optimal_summands(n);
    std::cout << summands.size() << '\n';
    for (size_t i = 0; i < summands.size(); ++i) {
        std::cout << summands[i] << ' ';
    }
}
