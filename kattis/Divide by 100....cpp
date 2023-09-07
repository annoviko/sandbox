#include <iostream>
#include <string>


bool is_greater_or_equal(const std::string& l, const std::string& r) {
    if (l.size() > r.size()) {
        return true;
    }
    else if (l.size() == r.size()) {
        for (int i = 0; i < l.size(); i++) {
            if (l[i] > r[i]) {
                return true;
            }
            else if (l[i] < r[i]) {
                return false;
            }
        }

        return true;
    }
    else {
        return false;
    }
}


int main() {
    std::string n, m;
    std::cin >> n >> m;

    std::string r;

    if (is_greater_or_equal(n, m)) {
        int divide_position = n.size() - m.size() + 1;
        std::cout << n.substr(0, divide_position);
        r = n.substr(divide_position);
    }
    else {
        int zeros = m.size() - n.size() - 1;
        std::cout << '0';
        r = std::string(zeros, '0') + n;
    }

    while (!r.empty() && r.back() == '0') {
        r.pop_back();
    }

    if (!r.empty()) {
        std::cout << '.' << r << std::endl;
    }

    return 0;
}