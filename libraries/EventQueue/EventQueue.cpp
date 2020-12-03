#include "EventQueue.hpp"


EventQueue::EventQueue(void) :
        queue(),
        mutex(),
        signal()
{ }


EventQueue::EventQueue(EventQueue && pOther) :
        queue(std::move(pOther.queue)),
        mutex(),
        signal()
{ }


EventQueue::~EventQueue(void)
{ }


void EventQueue::push(const void * const pEvent) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(pEvent);
    signal.notify_all();
}


void * EventQueue::pop(void) {
    std::unique_lock<std::mutex> lock(mutex);
    signal.wait(lock, [&] { return queue.size() > 0; });

    void * event = (void *) queue.front();
    queue.pop();

    return event;
}
