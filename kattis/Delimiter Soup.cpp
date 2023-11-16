#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>


const std::unordered_map<char, char> CLOSE_TO_OPEN = {
    { ']', '[' },
    { ')', '(' },
    { '}', '{' }
};


int main() {
    int n = 0;
    std::cin >> n;
    std::cin >> std::ws;

    std::string expression;
    std::getline(std::cin, expression);

    std::stack<char> brackets;
    for (int i = 0; i < n; i++) {
        char val = expression[i];

        switch (val) {
        case ' ':
            continue;

        case '[':
        case '{':
        case '(':
            brackets.push(val);
            continue;
        }

        char expected_bracket = CLOSE_TO_OPEN.find(val)->second;
        if (brackets.empty() || (expected_bracket != brackets.top())) {
            std::cout << val << ' ' << i << std::endl;
            return 0;
        }
        else {
            brackets.pop();
        }
    }

    std::cout << "ok so far" << std::endl;
    return 0;
}
