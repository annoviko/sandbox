class FooBar {
private:
    int n;

    std::mutex mutex;
    std::condition_variable cv;

    bool foo_ready = false;
    bool bar_ready = false;

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo(const function<void()>& printFoo) {
        
        for (int i = 0; i < n; ++i) {
            {
                std::lock_guard<std::mutex> guard(mutex);
                printFoo();
                foo_ready = true;
                bar_ready = false;
            }

            cv.notify_one();
        	
            {
                std::unique_lock<std::mutex> guard(mutex);
                cv.wait(guard, [this]() { return this->bar_ready; });
            }
        }
    }

    void bar(const function<void()>& printBar) {
        
        for (int i = 0; i < n; ++i) {
            {
                std::unique_lock<std::mutex> guard(mutex);
                cv.wait(guard, [this]() { return this->foo_ready; });

                printBar();

                foo_ready = false;
                bar_ready = true;
            }

            cv.notify_one();
        }
    }
};