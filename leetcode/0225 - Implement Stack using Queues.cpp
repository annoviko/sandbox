class MyStack {
private:
    std::queue<int> m_queue;
    
public:
    /** Initialize your data structure here. */
    MyStack() {
        
    }
    
    /** Push element x onto stack. */
    void push(int x) {
        m_queue.push(x);
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        const auto top = m_queue.back();
        
        if (m_queue.size() > 1) {
            std::queue<int> buffer;
            while (m_queue.size() > 1) {
                buffer.push(m_queue.front());
                m_queue.pop();
            }
            
            m_queue = std::move(buffer);
        }
        else {
            m_queue.pop();
        }
        
        return top;
    }
    
    /** Get the top element. */
    int top() {
        return m_queue.back();
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return m_queue.empty();
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack obj = new MyStack();
 * obj.push(x);
 * int param_2 = obj.pop();
 * int param_3 = obj.top();
 * bool param_4 = obj.empty();
 */