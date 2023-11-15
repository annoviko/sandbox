class Solution {
public:
    int evalRPN(const std::vector<std::string>& tokens) {
        std::stack<int> operands;

        for (const std::string& t : tokens) {
            if ((t.size() > 1) || std::isdigit(t[0])) {
                operands.push(std::stoi(t));
            }
            else {
                int op2 = operands.top();
                operands.pop();

                int op1 = operands.top();
                operands.pop();

                switch (t[0]) {
                case '+':
                    operands.push(op1 + op2);
                    break;

                case '-':
                    operands.push(op1 - op2);
                    break;

                case '*':
                    operands.push(op1 * op2);
                    break;

                case '/':
                    operands.push(op1 / op2);
                    break;
                }
            }
        }

        return operands.top();
    }
};
