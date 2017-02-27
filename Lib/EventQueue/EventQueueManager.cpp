#include "EventQueueManager.hpp"

#include "InternalAddress.hpp"


std::unordered_map<int, std::shared_ptr<EventQueue>> EventQueueManager::QUEUE_STORAGE =
        { { InternalAddress::DELAY_MANAGER,     std::shared_ptr<EventQueue>(new EventQueue()) },
          { InternalAddress::PATH_CONTROLLER,   std::shared_ptr<EventQueue>(new EventQueue()) } };


void EventQueueManager::put(const int pAddress, const void * const pEvent) {
    QUEUE_STORAGE[pAddress]->push(pEvent);
}


void * EventQueueManager::get(const int pAddress) {
    return QUEUE_STORAGE[pAddress].get()->pop();
}
