#ifndef STRING_TIME_HPP_
#define STRING_TIME_HPP_


#include <chrono>
#include <string>


class StringTime {
public:
	StringTime(void) = default;

	StringTime(const StringTime & pOther) = default;

	StringTime(StringTime && pOther) = default;

	~StringTime(void) = default;

public:
	static std::string getTime(const std::string & pFormat, const std::chrono::system_clock::time_point & pTimePoint);

	static std::string getCurrentTime(const std::string & pFormat);
};


#endif
