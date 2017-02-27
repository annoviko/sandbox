#include "Thread.hpp"


Thread::Thread(const std::string & name):
		m_stopFlag(true),
		m_thread(),
		m_threadName(name),
		printInterval(std::chrono::seconds(5))
{ }


Thread::Thread(const std::string & name, const unsigned int printInt):
		m_stopFlag(true),
		m_thread(),
		m_threadName(name),
		printInterval(std::chrono::seconds(printInt))
{ }


void Thread::start(void) {
	LOG(VIP) << "Starting " << m_threadName << " thread...";

	m_stopFlag = false;
	m_thread = std::thread(&Thread::run, this);

	LOG(VIP) << m_threadName << " is started at '" << StringTime::getCurrentTime("%d.%m %H:%M:%S") << "'.";
}


void Thread::localStart(void) { };


void Thread::localStop(void) { };


void Thread::run(void) {
	localStart();
	lastReportTime = std::chrono::system_clock::now();

	while(!m_stopFlag) {
		process();
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> timeSinceReport = currentTime - lastReportTime;
		if (timeSinceReport > printInterval) {
			LOG(INFO) << getThreadName() << " is alive at '" << StringTime::getCurrentTime("%d.%m %H:%M:%S") << "'.";
			lastReportTime = currentTime;
		}
	}
}


void Thread::stop(void) {
	m_stopFlag = true;
	localStop();
	m_thread.join();

	LOG(VIP) << m_threadName << " '" << std::this_thread::get_id() << "' is terminated at '" << StringTime::getCurrentTime("%d.%m %H:%M:%S") << "'.";
}


void Thread::sleep(const int usec) {
	std::this_thread::sleep_for(std::chrono::milliseconds(usec));
}
