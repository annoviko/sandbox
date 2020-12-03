#include <future>
#include <mutex>
#include <iostream>
#include <deque>
#include <thread>
#include <atomic>
#include <chrono>


const int AMOUNT_PRODUCTION = 20;
std::atomic<bool> stopFlag(false);

std::deque<int> production;
std::mutex		productLineMutex;


void producer(void) {
	static int id = 0;	
	
	for (int i = 0; i < AMOUNT_PRODUCTION; i++) {
		{
			std::lock_guard<std::mutex> lock(productLineMutex);
			production.push_back(++id);
		
			std::cout << "Producer: product '" << id << "' (size: '" << production.size() << "')." << std::endl;
		}
		
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	
	stopFlag.store(true);
}


void consumer(void) {
	for (;;) {
		{
			std::lock_guard<std::mutex> lock(productLineMutex);
			if (!production.empty()) {
				auto id = production.front();
				production.pop_front();
			
				std::cout << "Consumer '" << std::this_thread::get_id() << "': consume '" << id << "' (size: '" << production.size() << "')." << std::endl;
			}
			else {
				if (stopFlag.load()) { break; }
			
				std::cout << "Consumer '" << std::this_thread::get_id() << "': nothing to consume (size: '" << production.size() << "')." << std::endl;
			}
		}
		
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


int main(int argc, char *argv[]) {
	auto productionTask = std::async(std::launch::async, producer);
	auto consumtionTask1 = std::async(std::launch::async, consumer);
	auto consumtionTask2 = std::async(std::launch::async, consumer);
	
	return 0;
}
