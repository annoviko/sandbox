#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>


int main() {
    std::unordered_map<std::string, std::int64_t> vars;
    std::unordered_map<std::int64_t, std::string> vals_to_vars;

    for (std::string line; std::getline(std::cin, line);) {
        std::stringstream stream(line);

        std::string command;
        stream >> command;

        if (command == "def") {
            std::string name;
            std::int64_t value = 0;

            stream >> name >> value;

            auto iter = vars.find(name);
            if (iter != vars.cend()) {
                std::int64_t old_value_to_erase = iter->second;
                vals_to_vars.erase(old_value_to_erase);
            }

            vars[name] = value;
            vals_to_vars[value] = name;
        }
        else if (command == "clear") {
            vars.clear();
            vals_to_vars.clear();
        }
        else if (command == "calc") {
            std::int64_t result = 0;
            bool result_undefined = false;
            std::string text;

            std::string operation = "begin";

            while (true) {
                std::string var_name;

                stream >> var_name;
                text += var_name;

                auto iter = vars.find(var_name);
                if (iter == vars.cend()) {
                    result_undefined = true;
                }
                else {
                    if (operation == "+") {
                        result += iter->second;
                    }
                    else if (operation == "-") {
                        result -= iter->second;
                    }
                    else if (operation == "begin") {
                        result = iter->second;
                    }
                }

                stream >> operation;

                if (operation == "=") {
                    std::cout << text << " = ";

                    if (result_undefined) {
                        std::cout << "unknown" << std::endl;
                    }
                    else {
                        auto iter = vals_to_vars.find(result);
                        if (iter == vals_to_vars.cend()) {
                            std::cout << "unknown" << std::endl;
                        }
                        else {
                            std::cout << iter->second << std::endl;
                        }
                    }

                    break;
                }

                text += " " + operation + " ";
            }
        }
    }

    return 0;
}