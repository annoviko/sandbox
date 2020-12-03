#ifdef CONDITIONAL_VARIABLE_SOLUTION
class Foo {
private:
    std::mutex m_lock;
    std::condition_variable m_signal;

    int m_action_sequence = 1;

public:
    Foo() {

    }

    void first(std::function<void()> printFirst) {
        std::unique_lock<std::mutex> guard(m_lock);

        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();

        m_action_sequence = 2;
        m_signal.notify_all();
    }

    void second(std:function<void()> printSecond) {
        std::unique_lock<std::mutex> guard(m_lock);
        while (m_action_sequence != 2) {
            m_signal.wait(guard, [this]() { return m_action_sequence == 2; });
        }

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();

        m_action_sequence = 3;
        m_signal.notify_all();
}

    void third(std:function<void()> printThird) {
        std::unique_lock<std::mutex> guard(m_lock);
        while (m_action_sequence != 3) {
            m_signal.wait(guard, [this]() { return m_action_sequence == 3; });
        }

        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
#else
class Foo {
private:
    std::promise<bool> m_ready_first;
    std::promise<bool> m_ready_second;

public:
    Foo() {

    }

    void first(std::function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();

        m_ready_first.set_value(true);
    }

    void second(std::function<void()> printSecond) {
        m_ready_first.get_future().wait();

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();

        m_ready_second.set_value(true);
    }

    void third(std::function<void()> printThird) {
        m_ready_second.get_future().wait();

        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
#endif