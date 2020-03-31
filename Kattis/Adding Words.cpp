#include <iostream>
#include <string>
#include <unordered_map>


std::string read_value() {
    std::string value;
    std::cin >> value;
    return value;
}


std::string get_variable() {
    return read_value();
}


std::string get_sign() {
    return read_value();
}


std::string calculate(std::unordered_map <std::string, int> & variables, std::unordered_map <int, std::string> & value_name_dict) {
    std::string output;

    bool undefined = false;
    int result = 0;

    std::string name = get_variable();
    if (variables.count(name) == 0) {
        undefined = true; /* variable is not defined */
    }
    else {
        result = variables[name];
    }

    output += name + " ";

    while (true) {
        std::string sign = get_sign();
        output += sign + " ";

        if (sign == "=") {
            break;
        }

        std::string name = get_variable();
        output += name + " ";

        if (undefined) {
            continue;   /* variable is not defined - nothing to calculate */
        }

        if (variables.count(name) == 0) {
            undefined = true;
            continue;   /* variable is not defined */
        }

        if (sign == "+") {
            result += variables[name];
        }
        else if (sign == "-") {
            result -= variables[name];
        }
        else {
            break;
        }
    }

    if (undefined) {
        output += "unknown";
    }
    else {
        if (value_name_dict.count(result) == 0) {
            output += "unknown";
        }
        else {
            output += value_name_dict[result];
        }
    }

    return output;
}


int main() {
    std::unordered_map <std::string , int> variables;
    std::unordered_map <int, std::string> value_name_dict;

    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "def") {
            std::string name;
            std::cin >> name;

            int value = 0;
            std::cin >> value;

            variables[name] = value;
            value_name_dict[value] = name;
        }
        else if (command == "calc") {
            std::cout << calculate(variables, value_name_dict) << std::endl;
        }
        else {
            break;
        }
    }

    return 0;
}