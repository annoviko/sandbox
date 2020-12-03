#ifndef LOGGING_HPP
#define LOGGING_HPP


#include <iostream>
#include <string>
#include "StringTime.hpp"


class RcmOutBase { };
class RcmOut {
private:
	bool	alive;

public:
	RcmOut(void) : alive(true) { }

	RcmOut(RcmOut && pOther) : alive(true) { pOther.alive = false; }

	RcmOut(RcmOut const &) = delete;

	~RcmOut(void) { if(alive) std::cout << std::endl; }
};


template <typename Type>
RcmOut operator<<(RcmOut && pOutput, Type const & pContent) {
	std::cout << pContent;
	return std::move(pOutput);
}


template <typename Type>
RcmOut operator<<(RcmOutBase & pOutput, Type const & pContent) {
	return std::move(RcmOut() << pContent);
}



constexpr unsigned int FATAL = 6;
constexpr unsigned int VIP = 5;
constexpr unsigned int ERROR = 4;
constexpr unsigned int WARNING = 3;
constexpr unsigned int INFO = 2;
constexpr unsigned int TEMP = 1;
constexpr unsigned int DEBUG = 0;


#define COLOR_MARKER_USAGE      (true)


#define COLOR_MARKER_MESSAGE_FATAL    "\033[41;1m"
#define COLOR_MARKER_MESSAGE_VIP      "\033[32m"
#define COLOR_MARKER_MESSAGE_ERROR    "\033[31m"
#define COLOR_MARKER_MESSAGE_WARNING  "\033[33m"
#define COLOR_MARKER_MESSAGE_INFO     "\033[0m"
#define COLOR_MARKER_MESSAGE_TEMP     "\033[36;1m"
#define COLOR_MARKER_MESSAGE_DEBUG    "\037[0m"
#define COLOR_MARKER_MESSAGE_DEFAULT  "\033[0m"


extern volatile unsigned int    gLogLevel;

extern RcmOutBase               gRcmOut;


extern std::string setColor(unsigned int pSeverity);

extern std::string resetColor(void);


#define LOG(SEVERITY) if((SEVERITY) >= gLogLevel) gRcmOut << StringTime::getCurrentTime("%x %X ") << setColor(SEVERITY) << "[" << #SEVERITY << "] (" << __FILE__ << ":" << __LINE__ << "): " << resetColor()


#include <cassert>
#define ASSERT(CONDITION) assert(CONDITION)
#endif
