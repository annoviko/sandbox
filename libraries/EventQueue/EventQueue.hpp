#ifndef EVENT_QUEUE_HPP_
#define EVENT_QUEUE_HPP_


#include <condition_variable>
#include <mutex>
#include <queue>


class EventQueue {
private:
    std::queue<const void *>    queue;
    std::mutex                  mutex;
    std::condition_variable     signal;

public:
    EventQueue(void);

    EventQueue(const EventQueue & pOther) = delete;

    EventQueue(EventQueue && pOther);

    ~EventQueue(void);

public:
    void push(const void * const pEvent);

    void * pop(void);
};


#endif
