#include <mutex>
#include <condition_variable>
#include <functional>


class FooBar {
private:
    int n;

    std::mutex m_foo;
    std::mutex m_bar;

    std::condition_variable m_foo_ready;
    std::condition_variable m_bar_ready;

    bool m_is_foo_ready = false;
    bool m_is_bar_ready = false;

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo(std::function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            // printFoo() outputs "foo". Do not change or remove this line.
            printFoo();

            {
                std::lock_guard<std::mutex> lock(m_foo);
                m_is_foo_ready = true;
            }

            m_foo_ready.notify_one();

            {
                std::unique_lock<std::mutex> lock(m_bar);
                m_bar_ready.wait(lock, [this]() { return this->m_is_bar_ready; });
                m_is_bar_ready = false;
            }
        }
    }

    void bar(std::function<void()> printBar) {

        for (int i = 0; i < n; i++) {
            {
                std::unique_lock<std::mutex> lock(m_foo);
                m_foo_ready.wait(lock, [this]() { return this->m_is_foo_ready; });
                m_is_foo_ready = false;
            }

            // printBar() outputs "bar". Do not change or remove this line.
            printBar();

            {
                std::lock_guard<std::mutex> lock(m_bar);
                m_is_bar_ready = true;
            }

            m_bar_ready.notify_one();
        }
    }
};