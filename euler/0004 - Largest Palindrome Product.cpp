#include <iostream>
#include <cmath>

int main() {
    int cycle_counter = 0;
    int palindrome = 997799; /* max product is 998001 => biggest palindrome 997799 */
    int counter = 0;
    const int step = 1100;
    
    while (palindrome > 100001) {
        for (int i = std::sqrt(palindrome); palindrome / i < 1000; i--) {
            cycle_counter++;
            if (palindrome % i == 0) {
                std::cout << i << " x " << palindrome / i << " = " << palindrome << std::endl;
                std::cout << "cycle counter: " << cycle_counter << std::endl;
                return 0;
            }
        }

        /* generate next palindrome */
        counter++;
        if (counter % 10 == 0) {
            palindrome -= 110;
        }
        else {
            palindrome -= step;
        }
    }

    return 0;
}