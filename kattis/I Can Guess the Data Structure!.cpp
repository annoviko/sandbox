#include <iostream>
#include <stack>
#include <queue>


int main() {
    int n;
    while (std::cin >> n) {
        std::stack<int> st;
        bool is_st = true;

        std::queue<int> q;
        bool is_q = true;

        std::priority_queue<int> pq;
        bool is_pq = true;

        for (int i = 0; i < n; i++) {
            int op = 0;
            std::cin >> op;

            if (op == 1) {
                int value = 0;
                std::cin >> value;

                if (is_st) {
                    st.push(value);
                }

                if (is_q) {
                    q.push(value);
                }

                if (is_pq) {
                    pq.push(value);
                }
            }
            else {
                int expected = 0;
                std::cin >> expected;

                if (is_st) {
                    if (st.empty()) {
                        is_st = false;
                    }
                    else {
                        int value = st.top();
                        st.pop();

                        is_st = value == expected;
                    }
                }

                if (is_q) {
                    if (q.empty()) {
                        is_q = false;
                    }
                    else {
                        int value = q.front();
                        q.pop();

                        is_q = value == expected;
                    }
                }

                if (is_pq) {
                    if (pq.empty()) {
                        is_pq = false;
                    }
                    else {
                        int value = pq.top();
                        pq.pop();

                        is_pq = value == expected;
                    }
                }
            }
        }

        int answers = (is_st ? 1 : 0) + (is_q ? 1 : 0) + (is_pq ? 1 : 0);
        if (answers == 0) {
            std::cout << "impossible" << std::endl;
        }
        else if (answers == 1) {
            if (is_st) {
                std::cout << "stack" << std::endl;
            }
            else if (is_q) {
                std::cout << "queue" << std::endl;
            }
            else if (is_pq) {
                std::cout << "priority queue" << std::endl;
            }
        }
        else {
            std::cout << "not sure" << std::endl;
        }
    }

    return 0;
}
