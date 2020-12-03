#include <algorithm>
#include <list>


struct StackState {
    int m_value;
    int m_min;
};


class MinStack {
private:
    std::list<StackState> m_container;

public:
    MinStack() { }

    void push(int x) {
        int minimum = m_container.empty() ? x : std::min(getMin(), x);
        m_container.push_back({ x, minimum });
    }

    void pop() {
        m_container.pop_back();
    }

    int top() {
        return m_container.back().m_value;
    }

    int getMin() {
        return m_container.back().m_min;
    }
};