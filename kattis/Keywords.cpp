#include <string>
#include <iostream>
#include <unordered_set>


int main() {
    std::string temp;
    std::getline(std::cin, temp);

    int n = std::stoi(temp);

    std::unordered_set<std::string> keywords;

    for (int i = 0; i < n; i++) {
        std::string content;
        std::getline(std::cin, content);

        for (int j = 0; j < content.size(); j++) {
            if (std::isalpha(content[j])) {
                content[j] = std::tolower(content[j]);
            }
            else if (content[j] == '-') {
                content[j] = ' ';
            }
        }

        keywords.insert(content);
    }

    std::cout << keywords.size() << std::endl;

    return 0;
}