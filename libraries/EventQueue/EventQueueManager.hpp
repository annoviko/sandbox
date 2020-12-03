#ifndef EVENTQUEUEMANAGER_HPP_
#define EVENTQUEUEMANAGER_HPP_


#include <memory>
#include <queue>
#include <unordered_map>

#include "EventQueue.hpp"


class EventQueueManager {
private:
    EventQueueManager(void) = delete;

    EventQueueManager(const EventQueueManager & pOther) = delete;

    EventQueueManager(EventQueueManager && pOther) = delete;

    ~EventQueueManager(void) = delete;

private:
    static std::unordered_map<int, std::shared_ptr<EventQueue>> QUEUE_STORAGE;

public:
    static void put(const int pAddress, const void * const pEvent);

    static void * get(const int pAddress);
};


#endif
