#include <condition_variable>
#include <mutex>
#include <future>
#include <thread>
#include <iostream>
#include <deque>
#include <atomic>
#include <chrono>


const int AMOUNT_PRODUCTION = 20000;

std::deque<int> 			productLine;
std::mutex 					productLineMutex;
std::condition_variable 	productLineCondition;
std::atomic<bool> 			stopProduction(false);

void producer(void) {
	static int id = 0;
	
	for (int i = 0; i < AMOUNT_PRODUCTION; i++) {
		{
			std::lock_guard<std::mutex> lock(productLineMutex);
			productLine.push_back(++id);
			
			std::cout << "Producer '" << std::this_thread::get_id() << "': produce '" << id << "' (size '" << productLine.size() << "')." << std::endl;
		}
		
		productLineCondition.notify_all();
		
		std::this_thread::yield();
		//std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	
	stopProduction.store(true);
	productLineCondition.notify_all();
}


void consumer(void) {
	while(true) {
		{
			std::unique_lock<std::mutex> lock(productLineMutex);
			productLineCondition.wait(lock, []() { return !productLine.empty() || stopProduction.load(); });
			
			if (productLine.empty() && stopProduction.load()) { break; }
			
			auto product = productLine.front();
			productLine.pop_front();
			
			std::cout << "Consumer '" << std::this_thread::get_id() << "': consume '" << product << "' (size '" << productLine.size() << "')." << std::endl;
		}
		
		std::this_thread::yield();
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	std::cout << "Consumer '" << std::this_thread::get_id() << "' termination." << std::endl;
}


int main(int argc, char *argv[]) {
	auto p1 = std::async(std::launch::async, producer);
	
	auto c1 = std::async(std::launch::async, consumer);
	auto c2 = std::async(std::launch::async, consumer);
	auto c3 = std::async(std::launch::async, consumer);
	
	return 0;
}
