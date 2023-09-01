#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>


int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<std::uint64_t, int> combinations;

    for (int i = 0; i < n; i++) {
        std::vector<int> courses;

        for (int k = 0; k < 5; k++) {
            int course_code = 0;
            std::cin >> course_code;

            courses.push_back(course_code);
        }

        std::sort(std::begin(courses), std::end(courses));

        std::uint64_t key = 0;
        for (int k = 0; k < courses.size(); k++) {
            key *= 100;
            key += courses[k];
        }

        combinations[key]++;
    }

    int popularity = 0;
    int students = 0;
    for (const auto & pair : combinations) {
        if (pair.second > popularity) {
            popularity = pair.second;
            students = pair.second;
        }
        else if (pair.second == popularity) {
            students += pair.second;
        }
    }

    std::cout << students << std::endl;
    return 0;
}
