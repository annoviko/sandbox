#ifndef TCPMESSENGER_HPP_
#define TCPMESSENGER_HPP_


#include <string>

#include "Socket.hpp"


class TcpMessenger {
private:
	static const std::string    DEFAULT_MARKER_START;
	static const std::string    DEFAULT_MARKER_STOP;
	static const std::string    DEFAULT_MARKER_ESC;

private:
	Socket *        tcpSocket       = nullptr;
	std::string     markerStart     = "";
	std::string     markerStop      = "";
	std::string     markerEsc       = "";

	std::size_t     maxMarkerLength = 0;

public:
	TcpMessenger(void) = default;

	TcpMessenger(const TcpMessenger & pOther) = default;

	TcpMessenger(TcpMessenger && pOther) = default;

	explicit TcpMessenger(const Socket & pTcpSocket);

	TcpMessenger(const Socket & pTcpSocket, const std::string & pMarkerStart, const std::string & pMarkerStop, const std::string & pMarkerEsc);

	void operator()(const Socket & pTcpSocket);

	~TcpMessenger(void) = default;

public:
	std::string send(const std::string & pMessage) const;

	ssize_t receive(std::string & pMessage) const;

private:
	void construct(const Socket & pTcpSocket, const std::string & pMarkerStart, const std::string & pMarkerStop, const std::string & pMarkerEsc);

	void escape(const std::string & pEscapedMarker, std::string & pMessage) const;

	std::size_t eraseEscape(const std::size_t pPosition, std::string & pMessage) const;

	bool isEscaped(const std::size_t pPosition, const std::string & pMessage, const bool pEscaped = false) const;
};


#endif
