#include "TcpMessenger.hpp"

#include <algorithm>
#include <cstring>

#include "Logging.hpp"


const std::string	TcpMessenger::DEFAULT_MARKER_START = "<MSG_START>";

const std::string	TcpMessenger::DEFAULT_MARKER_STOP  = "<MSG_STOP>";

const std::string	TcpMessenger::DEFAULT_MARKER_ESC   = "<ESC>";


TcpMessenger::TcpMessenger(const Socket & pTcpSocket) {
	construct(pTcpSocket, DEFAULT_MARKER_START, DEFAULT_MARKER_STOP, DEFAULT_MARKER_ESC);
}


TcpMessenger::TcpMessenger(const Socket & pTcpSocket, const std::string & pMarkerStart, const std::string & pMarkerStop, const std::string & pMarkerEsc) {
	construct(pTcpSocket, pMarkerStart, pMarkerStop, pMarkerEsc);
}


void TcpMessenger::operator()(const Socket & pTcpSocket) {
	construct(pTcpSocket, DEFAULT_MARKER_START, DEFAULT_MARKER_STOP, DEFAULT_MARKER_ESC);
}


void TcpMessenger::construct(const Socket & pTcpSocket, const std::string & pMarkerStart, const std::string & pMarkerStop, const std::string & pMarkerEsc) {
	tcpSocket = (Socket *) &pTcpSocket;

	markerStart = pMarkerStart;
	markerStop = pMarkerStop;
	markerEsc = pMarkerEsc;

	std::vector<size_t> markers { markerStart.size(), markerStop.size(), markerEsc.size() };

	maxMarkerLength = *std::max_element(markers.begin(), markers.end());
	std::size_t minMarkerLength = *std::min_element(markers.begin(), markers.end());

	if ( (maxMarkerLength != 0) && (minMarkerLength == 0) ) {
		LOG(WARNING) << "It is forbidden to use selective empty markers - default markers are applied.";

		markerStart = DEFAULT_MARKER_START;
		markerStop = DEFAULT_MARKER_STOP;
		markerEsc = DEFAULT_MARKER_ESC;
	}
}


std::string TcpMessenger::send(const std::string & pMessage) const {
	if (maxMarkerLength != 0) {
		std::string message = pMessage;

		escape(markerEsc, message);
		escape(markerStart, message);
		escape(markerStop, message);

		message = markerStart + message + markerStop;
		tcpSocket->send(message);

		return message;
	}

	/* avoid lack copy */
	tcpSocket->send(pMessage);

	return pMessage;
}


ssize_t TcpMessenger::receive(std::string & pMessage) const {
	bool flagStart = false;
	bool flagStop = false;

	size_t cursor = 0;
	char receiveBuffer[512];

	do {
		std::memset(receiveBuffer, 0x00, sizeof(receiveBuffer));

		int messageLength = tcpSocket->receive((unsigned char *) receiveBuffer, sizeof(receiveBuffer));
		if (messageLength <= 0) {
			return messageLength;
		}

		pMessage.append(std::string((char *) receiveBuffer, (size_t) messageLength));
		if (maxMarkerLength == 0) {
			return messageLength;
		}

		/* Check for START FLAG marker */
		if (!flagStart) {
			std::size_t position = cursor;
			while ( (position = pMessage.find(markerStart, position)) != std::string::npos ) {
				if (!isEscaped(position, pMessage)) {
					if (position != 0) {
						LOG(WARNING) << "Ignore data '" << pMessage.substr(0, position) << "' before message start marker.";
					}

					/* Remove start marker */
					pMessage = pMessage.substr(position + markerStart.size(), pMessage.size() - position);
					cursor = position;

					flagStart = true;
					break;
				}

				cursor = position++;
			}
		}

		/* Check for STOP FLAG marker */
		if (flagStart && !flagStop) {
			std::size_t position = cursor;
			while ( (position = pMessage.find(markerStop, position)) != std::string::npos ) {
				if (!isEscaped(position, pMessage)) {
					if (position + markerStop.size() != pMessage.size()) {
						LOG(WARNING) << "Ignore data '" << pMessage.substr(position + markerStop.size()) << "' after message stop marker.";
					}

					/* Remove stop marker */
					std::string::iterator messageEnd = pMessage.begin() + position;
					pMessage.erase(messageEnd, pMessage.end());

					flagStop = true;
					break;
				}

				cursor = position++;
			}
		}

		/* Erase ESC markers */
		if (flagStop) {
			eraseEscape(0, pMessage);
		}
	}
	while(!flagStart || !flagStop);

	return pMessage.size();
}


void TcpMessenger::escape(const std::string & pEscapedMarker, std::string & pMessage) const {
	size_t position = 0;
	while ( (position = pMessage.find(pEscapedMarker, position)) != std::string::npos ) {
		pMessage.insert(position, markerEsc);
		position += markerEsc.size() + 1;
	}
}


std::size_t TcpMessenger::eraseEscape(const std::size_t pPosition, std::string & pMessage) const {
	std::size_t symbols = 0;
	std::size_t position = pPosition;
	while ( (position = pMessage.find(markerEsc, position)) != std::string::npos ) {
		pMessage.erase(position, markerEsc.size());
		position++;
		symbols++;
	}

	return symbols;
}


bool TcpMessenger::isEscaped(const std::size_t pPosition, const std::string & pMessage, const bool pEscaped) const {
	if (pPosition < markerEsc.size()) {
		return pEscaped;
	}

	if (pMessage.substr(pPosition - markerEsc.size(), markerEsc.size()) == markerEsc) {
		return isEscaped(pPosition - markerEsc.size(), pMessage, !pEscaped);
	}

	return pEscaped;
}
