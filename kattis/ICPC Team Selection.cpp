#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int k = 0;
        std::cin >> k;

        std::vector<int> students(3 * k);
        for (int j = 0; j < students.size(); j++) {
            std::cin >> students[j];
        }

        std::sort(students.begin(), students.end(), std::greater<>());

        int performance = 0;
        for (int j = 1, g = 0; g < k; j += 2, g++) {
            performance += students[j];
        }

        std::cout << performance << std::endl;
    }

    return 0;
}