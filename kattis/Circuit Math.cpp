#include <vector>
#include <string>
#include <stack>
#include <unordered_map>


#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<char, char> initial_values;
    int char_key = 'A';
    for (int i = 0; i < n; i++) {
        std::cin >> initial_values[char_key + i];
    }

    std::cin >> std::ws;

    std::string postfix;
    std::getline(std::cin, postfix);

    for (int i = 0, iv = 0; i < postfix.size(); i++) {
        if (postfix[i] >= 'A' && postfix[i] <= 'Z') {
            postfix[i] = initial_values[postfix[i]];
        }
    }

    std::stack<char> values;
    for (char c: postfix) {
        switch (c) {
            case ' ': break;

            case 'T':
            case 'F': {
                values.push(c);
                break;
            }

            case '*': {
                bool op2 = (values.top() == 'T');
                values.pop();

                bool op1 = (values.top() == 'T');
                values.pop();

                values.push((op1 && op2) ? 'T' : 'F');
                break;
            }

            case '+': {
                bool op2 = (values.top() == 'T');
                values.pop();

                bool op1 = (values.top() == 'T');
                values.pop();

                values.push((op1 || op2) ? 'T' : 'F');
                break;
            }

            case '-': {
                bool op = values.top() == 'T';
                values.pop();
                values.push(!op ? 'T' : 'F');
                break;
            }
        }
    }

    std::cout << values.top() << std::endl;

    return 0;
}