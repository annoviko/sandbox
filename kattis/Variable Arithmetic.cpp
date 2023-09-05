#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>


bool is_integer(const std::string& value) {
    return !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit);
}


int main() {
    std::unordered_map<std::string, int> vars;

    for (std::string line; std::getline(std::cin, line) && line != "0";) {
        std::stringstream stream(line);

        std::string lvalue;
        char operation = 0;

        stream >> lvalue >> operation;

        if (operation == '=') {
            std::string rvalue;
            stream >> rvalue;

            if (is_integer(rvalue)) {
                vars[lvalue] = std::stoi(rvalue);
            }
        }
        else {
            std::vector<std::string> unknown;
            int iresult = 0;

            if (is_integer(lvalue)) {
                iresult = std::stoi(lvalue);
            }
            else {
                auto iter = vars.find(lvalue);
                if (iter == vars.cend()) {
                    unknown.push_back(lvalue);
                }
                else {
                    iresult = iter->second;
                }
            }

            for (std::string rvalue; stream >> rvalue;) {
                if (rvalue == "+") {
                    continue;
                }

                if (is_integer(rvalue)) {
                    iresult += std::stoi(rvalue);
                    continue;
                }

                auto iter = vars.find(rvalue);
                if (iter == vars.cend()) {
                    unknown.push_back(rvalue);
                    continue;
                }

                iresult += iter->second;
            }

            if (iresult != 0) {
                std::cout << iresult;
            }

            if (iresult == 0 && unknown.empty()) {
                std::cout << iresult;
            }

            for (int i = 0; i < unknown.size(); i++) {
                if (iresult != 0 || i != 0) {
                    std::cout << " + ";
                }
                std::cout << unknown[i];
            }

            std::cout << std::endl;
        }
    }

    return 0;
}