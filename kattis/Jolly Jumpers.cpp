#include <vector>
#include <iostream>


int main() {
    int n = 0;
    while (std::cin >> n && n > 0) {
        std::vector<bool> flags(n - 1, false);
        int counter = 0;
        
        int op1;
        std::cin >> op1;

        for (int i = 1; i < n; i++) {
            int op2;
            std::cin >> op2;

            int diff = std::abs(op1 - op2) - 1;
            if (diff >= 0 && diff < flags.size()) {
                if (!flags[diff]) {
                    flags[diff] = true;
                    counter++;
                }
            }

            op1 = op2;
        }

        if (counter == n - 1) {
            std::cout << "Jolly" << std::endl;
        }
        else {
            std::cout << "Not jolly" << std::endl;
        }
    }

    return 0;
}