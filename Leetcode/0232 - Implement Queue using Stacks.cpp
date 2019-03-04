class MyQueue {
private:
    std::stack<int> direct;
    std::stack<int> reverse;
    
public:
    /** Initialize your data structure here. */
    MyQueue() {
        
    }
    
    /** Push element x to the back of queue. */
    void push(int x) {
        while (!reverse.empty()) {
            direct.push(reverse.top());
            reverse.pop();
        }
        
        direct.push(x);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        while (!direct.empty()) {
            reverse.push(direct.top());
            direct.pop();
        }
        
        int result = reverse.top();
        reverse.pop();
        return result;
    }
    
    /** Get the front element. */
    int peek() { 
        while (!direct.empty()) {
            reverse.push(direct.top());
            direct.pop();
        }
        
        return reverse.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return direct.empty() && reverse.empty();
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue obj = new MyQueue();
 * obj.push(x);
 * int param_2 = obj.pop();
 * int param_3 = obj.peek();
 * bool param_4 = obj.empty();
 */
 