#include <iostream>
#include <cstdint>
#include <vector>


int main() {
    int t = 0;
    std::cin >> t;

    for (int _t = 0; _t < t; _t++) {
        int joke_confirmation = 0;

        int gr_a, gr_b;
        std::cin >> gr_a >> gr_b;

        long double avg_a = 0;
        long double avg_b = 0;

        std::vector<int> it_students;

        for (int i = 0; i < gr_a; i++) {
            int iq;
            std::cin >> iq;

            avg_a += iq;
            it_students.push_back(iq);
        }

        for (int i = 0; i < gr_b; i++) {
            int iq;
            std::cin >> iq;

            avg_b += iq;
        }

        avg_a /= gr_a;
        avg_b /= gr_b;

        for (int i = 0; i < it_students.size(); i++) {
            if (it_students[i] < avg_a && it_students[i] > avg_b) {
                joke_confirmation++;
            }
        }

        std::cout << joke_confirmation << std::endl;
    }

    return 0;
}