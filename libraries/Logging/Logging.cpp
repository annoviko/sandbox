#include "Logging.hpp"


#ifdef FXRCM_UNIT_TESTING
volatile unsigned int gLogLevel = (FATAL + 1);
#else
volatile unsigned int gLogLevel = (TEMP);
#endif


RcmOutBase gRcmOut;


std::string setColor(unsigned int pSeverity) {
	if (COLOR_MARKER_USAGE) {
		switch(pSeverity) {
		case VIP:
			return COLOR_MARKER_MESSAGE_VIP;
		case FATAL:
			return COLOR_MARKER_MESSAGE_FATAL;
		case ERROR:
			return COLOR_MARKER_MESSAGE_ERROR;
		case WARNING:
			return COLOR_MARKER_MESSAGE_WARNING;
		case INFO:
			return COLOR_MARKER_MESSAGE_INFO;
		case TEMP:
			return COLOR_MARKER_MESSAGE_TEMP;
		case DEBUG:
			return COLOR_MARKER_MESSAGE_DEBUG;
		default:
			return COLOR_MARKER_MESSAGE_DEFAULT;
		}
	}

	return "";
}


std::string resetColor(void) {
	if (COLOR_MARKER_USAGE) {
		return COLOR_MARKER_MESSAGE_DEFAULT;
	}

	return "";
}
