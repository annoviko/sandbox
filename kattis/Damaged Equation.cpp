#include <climits>
#include <iostream>
#include <vector>


const int INVALID_VALUE = INT_MAX;


int get_result(int l, int r, char op) {
    switch (op) {
    case '*':
        return l * r;
    case '+':
        return l + r;
    case '-':
        return l - r;
    case '/':
        if (r == 0) {
            return INVALID_VALUE;
        }

        return l / r;
    }

    return INVALID_VALUE;
}


int main() {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;

    std::vector<char> operations = { '*', '+', '-', '/' };
    bool solution_found = false;

    for (int i = 0; i < operations.size(); i++) {
        int lres = get_result(a, b, operations[i]);
        if (lres == INVALID_VALUE) {
            continue;
        }

        for (int j = 0; j < operations.size(); j++) {
            int rres = get_result(c, d, operations[j]);
            if (rres == INVALID_VALUE) {
                continue;
            }

            if (lres == rres) {
                std::cout << a << ' ' << operations[i] << ' ' << b << " = " << c << ' ' << operations[j] << ' ' << d << std::endl;
                solution_found = true;
            }
        }
    }

    if (!solution_found) {
        std::cout << "problems ahead" << std::endl;
    }

    return 0;
}