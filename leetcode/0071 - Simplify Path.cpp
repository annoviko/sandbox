#include <string>
#include <stack>


class Solution {
public:
    void handle_name(const std::string& name, std::stack<std::string>& simple_path) {
        if (name.empty() || (name == ".")) {
            return;
        }
        else if (name == "..") {
            if (!simple_path.empty()) {
                simple_path.pop();
            }
        }
        else {
            simple_path.push(name);
        }
    }

    std::string simplifyPath(std::string path) {
        std::stack<std::string> simple_path;
        std::string name;
        for (char symbol : path) {
            if (symbol != '/') {
                name.append(1, symbol);
                continue;
            }

            handle_name(name, simple_path);
            name.clear();
        }

        handle_name(name, simple_path);

        std::string result;
        while (!simple_path.empty()) {
            std::string name = simple_path.top();
            simple_path.pop();

            if (result.empty()) {
                result = name;
            }
            else {
                result = name + "/" + result;
            }
        }

        return "/" + result;
    }
};