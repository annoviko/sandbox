#include <iostream>
#include <string>
#include <unordered_map>


int main() {
    int n = 0;
    std::string code, guess;

    std::cin >> n >> code >> guess;

    int r = 0, s = 0;

    int rem_code[26] = { 0 };

    for (int i = 0; i < n; i++) {
        if (code[i] == guess[i]) {
            r++;
            guess[i] = 0;
        }
        else {
            rem_code[code[i] - 'A']++;
        }
    }

    for (int i = 0; i < n; i++) {
        if (guess[i] == 0) {
            continue;
        }
        else {
            int& r = rem_code[guess[i] - 'A'];
            if (r > 0) {
                r--;
                s++;
            }
        }
    }

    std::cout << r << ' ' << s << std::endl;

    return 0;
}