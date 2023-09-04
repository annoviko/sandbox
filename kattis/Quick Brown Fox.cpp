#include <iostream>
#include <string>


int main() {
    std::string input;
    std::getline(std::cin, input);

    int n = std::stoi(input);

    for (int i = 0; i < n; i++) {
        int counter[255] = { 0 };

        std::string input;
        std::getline(std::cin, input);

        for (const char val : input) {
            const char index = std::tolower(val);
            counter[index]++;
        }

        bool failure = false;
        for (int j = 'a'; j <= 'z'; j++) {
            if (counter[j] == 0) {
                if (!failure) {
                    std::cout << "missing ";
                    failure = true;
                }

                std::cout << (char)j;
            }
        }

        if (!failure) {
            std::cout << "pangram" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
    }

    return 0;
}