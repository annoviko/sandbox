#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

using std::vector;

int get_majority_element(vector<int> &a, int left, int right) {
    if (left == right) return -1;
    if (left + 1 == right) return a[left];

    int middle = left + (right - left) / 2;

    int majority_left = get_majority_element(a, left, middle);
    int majority_right = get_majority_element(a, middle, right);

    if (majority_left == -1 && majority_right == -1) {
        return -1;
    }

    int score_left = 0, score_right = 0;
    for (int i = left; i < right; i++) {
        if (majority_left == a[i]) {
            score_left++;
        }
        else if (majority_right == a[i]) {
            score_right++;
        }
    }

    const int half = (right - left) / 2;
    if (score_left > half) {
        return majority_left;
    }
    else if (score_right > half) {
        return majority_right;
    }
    else {
        return -1;
    }
}

int get_majority_element_no_dac(vector<int> &a) {
    std::unordered_map<int, int> votes;
    for (auto key : a) {
        votes[key]++;
    }

    int majority = 0;
    for (auto pair : votes) {
        majority = std::max(majority, pair.second);
    }

    return (majority > a.size() / 2) ? 1 : 0;
}


void stress_test() {
    while (true) {
        int n = std::rand() % 10 + 1;
        std::vector<int> values(n);
        for (int i = 0; i < values.size(); i++) {
            values[i] = rand() % 5 + 1;
        }

        bool result1 = get_majority_element_no_dac(values);
        bool result2 = (get_majority_element(values, 0, values.size()) != -1);

        if (result1 != result2) {
            std::cout << "FAILURE" << std::endl;
            std::terminate();
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
    std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
