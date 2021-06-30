#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>

using std::vector;
using std::swap;

std::pair<int, int> partition3(vector<int> &a, int l, int r) {
    int x = a[l];

    int left_border = l;
    int right_border = l + 1;

    for (int i = l + 1; i <= r; i++) {
        if (a[i] == x) {
            swap(a[right_border], a[i]);
            right_border++;
        }
        else if (a[i] < x) {
            swap(a[left_border], a[right_border]);

            if (i != right_border) {
                swap(a[left_border], a[i]);
            }

            left_border++;
            right_border++;
        }
    }

    return {left_border, right_border};
}

void randomized_quick_sort(vector<int> &a, int l, int r) {
    if (l >= r) {
        return;
    }

    int k = l + rand() % (r - l + 1);
    swap(a[l], a[k]);
    std::pair<int, int> borders = partition3(a, l, r);

    randomized_quick_sort(a, l, borders.first);
    randomized_quick_sort(a, borders.second, r);
}

void stress_test() {
    while (true) {
        int n = std::rand() % 5 + 1;

        std::vector<int> vec1(n, 0);
        for (std::size_t i = 0; i < n; i++) {
            vec1[i] = std::rand() % 10;
        }

        std::vector<int> vec2 = vec1;
        std::vector<int> initial = vec1;

        std::sort(vec1.begin(), vec1.end());
        randomized_quick_sort(vec2, 0, vec2.size() - 1);

        if (vec1 != vec2) {
            std::cout << "FAILURE" << std::endl;
            for (auto elem : initial) {
                std::cout << elem << ' ';
            }
            std::cout << std::endl;
            exit(-1);
        }
        else {
            std::cout << "SUCCESS" << std::endl;
        }
    }
}

int main() {
    //stress_test();
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
    }
    randomized_quick_sort(a, 0, a.size() - 1);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << ' ';
    }
}
