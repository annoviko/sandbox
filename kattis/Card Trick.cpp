#include <iostream>
#include <list>
#include <vector>


int main() {
    int c;
    std::cin >> c;

    for (int i = 0; i < c; i++) {
        int n = 0;
        std::cin >> n;

        std::vector<int> initial_positions(n);
        std::list<int> queue;   /* store card as initial positions */
        for (int pos = 0; pos < n; pos++) {
            queue.push_back(pos);
        }

        for (int j = 0; j < n; j++) {
            for (int k = 0; k < j + 1; k++) {
                /* move k times card to bottom */
                int top_card = queue.front();
                queue.pop_front();

                queue.push_back(top_card);
            }

            /* take card at the top and assign value */
            int top_card_initial_position = queue.front();
            queue.pop_front();

            initial_positions[top_card_initial_position] = j + 1;
        }

        bool is_first = true;
        for (int k = 0; k < initial_positions.size(); k++) {
            if (!is_first) {
                std::cout << ' ';
            }

            std::cout << initial_positions[k];
            is_first = false;
        }

        std::cout << std::endl;
    }

    return 0;
}
