#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>


std::unordered_map<std::string, int> vars;


bool is_integer(const std::string& value) {
    for (int i = 0; i < value.size(); i++) {
        const char val = value[i];

        if ((val == '-') && (i == 0)) {
            continue;
        }

        if (val < '0' || val > '9') {
            return false;
        }
    }

    return true;
}


int get_value(const std::string& var_or_value) {
    if (is_integer(var_or_value)) {
        return std::stoi(var_or_value);
    }

    return vars[var_or_value];
}



int add(const std::string& l, const std::string& r) {
    return get_value(l) + get_value(r);
}


int sub(const std::string& l, const std::string& r) {
    return get_value(l) - get_value(r);
}


int mul(const std::string& l, const std::string& r) {
    return get_value(l) * get_value(r);
}


int div(const std::string& l, const std::string& r) {
    return get_value(l) / get_value(r);
}


int main() {
    std::unordered_map<int, std::vector<std::string>> instruction_set;
    std::vector<int> label_order;

    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        std::stringstream stream(line);
        
        int label = 0;
        std::vector<std::string> args;

        stream >> label;
        
        std::string command;
        stream >> command;
        args.push_back(command);

        if (command != "PRINT" && command != "PRINTLN") {
            for (std::string argument; stream >> argument;) {
                args.push_back(argument);
            }
        }
        else {
            int position = line.find('"');
            if (position != -1) {
                args.push_back(line.substr(position));
            }
            else {
                std::string var_name;
                stream >> var_name;
                args.push_back(var_name);
            }
        }

        instruction_set[label] = args;
        label_order.push_back(label);
    }

    std::sort(label_order.begin(), label_order.end());

    std::unordered_map<int, int> label_to_index;
    for (int i = 0; i < label_order.size(); i++) {
        label_to_index[label_order[i]] = i;
    }

    for (int i = 0; i < label_order.size();) {
        const auto& args = instruction_set[label_order[i]];

        const std::string& command = args[0];

        if (command == "LET") { /* LET A = D or LET A = D + C */
            const std::string& var_name = args[1];

            const std::string& lvalue = args[3];
            int value = get_value(lvalue);

            if (args.size() > 4) {
                char operation = args[4][0];
                const std::string& rvalue = args[5];

                switch (operation) {
                case '+':
                    value = add(lvalue, rvalue);
                    break;

                case '-':
                    value = sub(lvalue, rvalue);
                    break;

                case '*':
                    value = mul(lvalue, rvalue);
                    break;

                case '/':
                    value = div(lvalue, rvalue);
                    break;
                }
            }

            vars[var_name] = value;
            i += 1;
        }
        else if (command == "PRINT" || command == "PRINTLN") {
            if (args[1][0] != '"') {
                std::cout << get_value(args[1]);
            }
            else {
                std::string content = args[1].substr(1, args[1].size() - 2);
                std::cout << content;
            }

            if (command == "PRINTLN") {
                std::cout << std::endl;
            }

            i += 1;
        }
        else if (command == "PRINTLN") {
            std::string content = args[1].substr(1, args[1].size() - 2);
            std::cout << content << std::endl;
            i += 1;
        }
        else if (command == "IF") {  /* IF 1 = 1 THEN GOTO 180 */
            int lvalue = get_value(args[1]);
            int rvalue = get_value(args[3]);

            std::string operation = args[2];

            int label = std::stoi(args[6]);

            bool is_true = false;
            if (operation == "=") {
                is_true = (lvalue == rvalue);
            }
            else if (operation == ">") {
                is_true = (lvalue > rvalue);
            }
            else if (operation == "<") {
                is_true = (lvalue < rvalue);
            }
            else if (operation == "<>") {
                is_true = (lvalue != rvalue);
            }
            else if (operation == "<=") {
                is_true = (lvalue <= rvalue);
            }
            else if (operation == ">=") {
                is_true = (lvalue >= rvalue);
            }

            if (is_true) {
                i = label_to_index[label];
            }
            else {
                i++;
            }
        }
        else {
            throw std::invalid_argument("unkown argument");
        }
    }

    return 0;
}