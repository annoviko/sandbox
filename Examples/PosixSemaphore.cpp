#include <iostream>
#include <deque>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>


std::deque<int> productLine;


const int AMOUNT_PRODUCTION = 100;
const int STEP_PRODUCTION = 1;

bool doneFlag = false;

sem_t * productSemaphore;
sem_t * productMutex;

void * producer(void * args) {
	static int productId = 0;

	std::cout << "Producer is started..." << std::endl;	

	while (productId < AMOUNT_PRODUCTION) {
		sem_wait(productMutex);

		productLine.push_back(++productId);
		std::cout << "Producer: product '" << productId << "' is produced (size '" << productLine.size() << "')." << std::endl;

		sem_post(productSemaphore);
		sem_post(productMutex);

		pthread_yield();
	}

	doneFlag = true;

	pthread_exit(0);
}


void * consumer(void * args) {
	static int productNumber = 0;

	std::cout << "Consumer is started..." << std::endl;

	while(productNumber < AMOUNT_PRODUCTION) {
		sem_wait(productSemaphore);
		sem_wait(productMutex);

		auto productId = productLine.front();
		productLine.pop_front();
		
		std::cout << "Consumer: product '" << productId << "' is consumed (size '" << productLine.size() << "')." << std::endl;

		productNumber++;
		sem_post(productMutex);
		
		pthread_yield();
	}

	pthread_exit(0);
}


int main(int argc, char *argv[]) {
	const char SEM_NAME[] = "SEM_PRODUCT_1";
	const char SEM_MUTEX_NAME[] = "SEM_PRODUCT_MUTEX_1";

	productSemaphore = sem_open(SEM_NAME, O_CREAT, 0644, 0);
	if (productSemaphore == SEM_FAILED) {
		std::cout << "ERROR: Impossible to create product semaphore (cause: '" << errno << "')." << std::endl;
		return -1;	
	}

	productMutex = sem_open(SEM_MUTEX_NAME, O_CREAT, 0644, 1);
	if (productMutex == SEM_FAILED) {
		std::cout << "ERROR: Impossible to create product mutex semaphore (cause: '" << errno << "')." << std::endl;
		return -1;
	}

	pthread_t consumerThread;
	pthread_t producerThread;

	pthread_create(&consumerThread, NULL, &consumer, (void *) NULL);
	pthread_create(&producerThread, NULL, &producer, (void *) NULL);

	pthread_join(consumerThread, NULL);
	pthread_join(producerThread, NULL);

	sem_close(productSemaphore);
	sem_unlink(SEM_NAME);
	sem_close(productMutex);
	sem_unlink(SEM_MUTEX_NAME);

	return 0;
}


