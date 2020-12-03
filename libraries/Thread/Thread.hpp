#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <thread>
#include <typeinfo>


#include "StringTime.hpp"
#include "Logging.hpp"

class Thread {
private:
	bool			m_stopFlag;
	std::thread		m_thread;
	std::string		m_threadName;

	std::chrono::system_clock::time_point lastReportTime;
	std::chrono::seconds printInterval;

public:
	explicit Thread(const std::string & name);

	explicit Thread(const std::string & name, const unsigned int printInt);

	Thread(const Thread &) = delete;

	virtual ~Thread() = default;

public:
	Thread & operator=(const Thread & pOther) = delete;

	Thread & operator=(Thread && pOther) = default;

public:
	virtual void start(void) final;

	virtual void localStart(void);

	virtual void localStop(void);

	virtual void process(void) = 0;

	virtual void run(void) final;

	virtual void stop(void) final;

public:
	static void sleep(const int usec);

public:
	inline void setStop(void) { m_stopFlag = true; }

	const std::string & getThreadName(void) const { return m_threadName; }
};


#endif
