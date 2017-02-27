#include "StringTime.hpp"

#include <cstring>


std::string StringTime::getTime(const std::string & pFormat, const std::chrono::system_clock::time_point & pTimePoint) {
	std::time_t curTime = std::chrono::system_clock::to_time_t(pTimePoint);
	std::tm sCurTime = *std::localtime(&curTime);

	char timeLine[64];
	std::memset(timeLine, 0x00, sizeof(timeLine));

	strftime(timeLine, sizeof(timeLine), pFormat.c_str(), &sCurTime);

	return std::string((char *) timeLine);
}


std::string StringTime::getCurrentTime(const std::string & pFormat) {
	return StringTime::getTime(pFormat, std::chrono::system_clock::now());
}
