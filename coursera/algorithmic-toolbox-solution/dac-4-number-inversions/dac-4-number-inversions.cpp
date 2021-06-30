#include <iostream>
#include <vector>

using std::vector;

long long get_number_of_inversions(vector<int> &a, size_t left, size_t right) {
    long long number_of_inversions = 0;
    if (right <= left + 1) return number_of_inversions;

    size_t len = right - left;
    size_t ave = left + len / 2;

    number_of_inversions += get_number_of_inversions(a, left, ave);
    number_of_inversions += get_number_of_inversions(a, ave, right);

    vector<int> result(len);
    size_t lborder = left, rborder = ave;
    for (std::size_t i = 0; i < len; i++) {
        bool ldone = lborder == ave;
        bool rdone = rborder == right;

        if (!ldone && !rdone) {
            if (a[lborder] <= a[rborder]) {
                result[i] = a[lborder++];
            }
            else {
                number_of_inversions += ave - lborder;
                result[i] = a[rborder++];
            }
        }
        else if (!ldone) {
            result[i] = a[lborder++];
        }
        else {
            result[i] = a[rborder++];
        }
    }

    for (std::size_t i = 0; i < len; i++) {
        a[i + left] = result[i];
    }

    return number_of_inversions;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); i++) {
        std::cin >> a[i];
    }
    vector<int> b(a.size());
    std::cout << get_number_of_inversions(a, 0, a.size()) << '\n';
}
