#include "Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>

#include "Logging.hpp"


Socket::Socket(void) :
		socket(-1)
{ }


Socket::Socket(const Protocol protocol) { createSocket(protocol); }


Socket::Socket(const int pStyle, const int pProtocol) { createSocket(pStyle, pProtocol); }


Socket::Socket(const int pSocketDescriptor) :
		socket(pSocketDescriptor)
{ }


Socket::Socket(Socket && pSocket) :
		socket(pSocket.socket)
{ pSocket.socket = -1; }


Socket::~Socket(void) { close(); }


void Socket::createSocket(const Protocol protocol) {
	switch (protocol)
	{
		case (Protocol::UDP): createSocket(SOCK_DGRAM, IPPROTO_UDP);
			break;
		case (Protocol::TCP): createSocket(SOCK_STREAM, IPPROTO_TCP);
			break;
	}
}

void Socket::createSocket(const int pStyle, const int pProtocol) {
	socket = ::socket(AF_INET, pStyle, pProtocol);
	if (socket < 0) {
		LOG(ERROR) << "Impossible to create socket (style '" << pStyle << "', protocol '" << pProtocol << "', cause '" << errno << "').";
		socket = -1;
	}
	else {
		setAddressReusage(true);
	}
}


int Socket::bind(const std::string & pAddress, const int pPort) {
	struct sockaddr_in socketAddress;
	std::memset((void *) &socketAddress, 0x00, sizeof(socketAddress));

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(pPort);
	inet_pton(AF_INET, pAddress.c_str(), &socketAddress.sin_addr);

	int result = ::bind(socket, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
	if (result < 0) {
		LOG(ERROR) << "Impossible to bind socket (" << socket << ") to '" << pAddress << ":" << pPort << "', cause '" << errno << "'.";
	}

	return (result < 0) ? -1 : 0;
}


void Socket::close(void) {
	if (socket > 0) {
		::close(socket);
		socket = -1;
	}
}


void Socket::send(const std::string & pMessage) const {
	send((const unsigned char * const) pMessage.c_str(), pMessage.size());
}


void Socket::send(const unsigned char * const pMessage, const size_t pLength) const {
	int result = ::send(socket, pMessage, pLength, 0);
	if (result < 0) {
		LOG(ERROR) << "Impossible to send data to '" << getPeerAddress() << ":" << getPeerPort() << "' (cause: '" << errno << "').";
	}
}


void Socket::sendTo(const std::string & pMessage, const std::string & pAddress, const int pPort) const {
	sendTo((const unsigned char * const) pMessage.c_str(), pMessage.size(), pAddress, pPort);
}


void Socket::sendTo(const unsigned char * const pMessage, const size_t pLength, const std::string & pAddress, const int pPort) const {
	struct sockaddr_in socketAddress;
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));

	socketAddress.sin_port = htons(pPort);
	socketAddress.sin_family = AF_INET;
	inet_pton(AF_INET, pAddress.c_str(), &socketAddress.sin_addr);

	int result = ::sendto(socket, pMessage, pLength, 0, (struct sockaddr *) &(socketAddress), sizeof(socketAddress));
	if (result < 0) {
		LOG(ERROR) << "Impossible to send data to '" << pAddress << ":" << pPort << "' (cause: '" << errno << "').";
	}
}


ssize_t Socket::receive(unsigned char * pMessage, const size_t pLength) const {
	ssize_t messageLength = ::recv(socket, pMessage, pLength, 0);
	if ( (messageLength < 0) && (errno != EAGAIN) ) {
		LOG(ERROR) << "Impossible to receive data (cause: '" << errno << "').";
	}

	return messageLength;
}


ssize_t Socket::receiveFrom(unsigned char * pMessage, const size_t pLength, std::string & pAddress, int & pPort) const {
	struct sockaddr_in socketAddress;
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));
	socklen_t addressClientLength = sizeof(socketAddress);

	ssize_t messageLength = ::recvfrom(socket, pMessage, pLength, 0, (struct sockaddr *) &socketAddress, &addressClientLength);
	if ( (messageLength < 0) && (errno != EAGAIN) ) {
		LOG(ERROR) << "Impossible to receive datagram packet from '" << getAddress() << ":" << getPort() << "' (cause: '" << errno << "').";
	}

	char ipBuffer[INET_ADDRSTRLEN];
	std::memset(&ipBuffer, 0x00, sizeof(ipBuffer));
	inet_ntop(AF_INET, &(socketAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);

	pAddress = std::string(ipBuffer);
	pPort = ntohs(socketAddress.sin_port);

	return messageLength;
}


int Socket::accept(Socket & pTransport) {
	struct sockaddr_in clientAddress;
	std::memset(&clientAddress, 0x00, sizeof(clientAddress));
	socklen_t clientAddressLen = sizeof(clientAddress);

	int clientSocket = ::accept(socket, (struct sockaddr *) &clientAddress, &clientAddressLen);
	if (clientSocket < 0) {
		if ( (errno != EAGAIN) && (errno != EWOULDBLOCK) ) {
			LOG(ERROR) << "Impossible to accept input connection (cause '" << errno << "').";
		}
	}

	pTransport = std::move((Socket(clientSocket)));

	return (clientSocket < 0) ? -1 : 0;
}


int Socket::listen(const int pAmountConnections) {
	int result = ::listen(socket, pAmountConnections);
	if (result < 0) {
		LOG(ERROR) << "Impossible to set amount of incoming connections (cause '" << errno << "').";
	}

	return (result < 0) ? -1 : 0;
}


int Socket::connect(const std::string & pAddress, const int pPort) {
	struct sockaddr_in socketAddress;
	std::memset((void *) &socketAddress, 0x00, sizeof(socketAddress));

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(pPort);
	inet_pton(AF_INET, pAddress.c_str(), &socketAddress.sin_addr);

	int result = ::connect(socket, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
	if (result < 0) {
		LOG(ERROR) << "Impossible to connect to remote address '" << pAddress << ":" << pPort << "' (cause '" << errno << "').";
	}

	return (result < 0) ? -1 : 0;
}


std::string Socket::getAddress(void) const {
	struct sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(socketAddress);
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));

	int result = getsockname(socket, (struct sockaddr *) &socketAddress, &socketAddressLength);
	if (result < 0) {
		LOG(ERROR) << "Impossible to obtain socket information for getting address cause, '" << errno << "'.";
		return "";
	}

	char ipBuffer[INET_ADDRSTRLEN];
	std::memset(&ipBuffer, 0x00, sizeof(ipBuffer));

	inet_ntop(AF_INET, &(socketAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);

	return std::string(ipBuffer);
}


int Socket::getPort(void) const {
	struct sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(socketAddress);
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));

	int result = getsockname(socket, (struct sockaddr *) &socketAddress, &socketAddressLength);
	if (result < 0) {
		LOG(ERROR) << "Impossible to obtain socket information for getting port, cause '" << errno << "'.";
		return -1;
	}

	return ntohs(socketAddress.sin_port);
}


std::string Socket::getPeerAddress(void) const {
	struct sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(socketAddress);
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));

	int result = ::getpeername(socket, (struct sockaddr *) &socketAddress, &socketAddressLength);
	if (result < 0) {
		LOG(ERROR) << "Impossible to obtain socket information for getting peer address, cause '" << errno << "'.";
		return "";
	}

	char ipBuffer[INET_ADDRSTRLEN];
	std::memset(&ipBuffer, 0x00, sizeof(ipBuffer));

	inet_ntop(AF_INET, &(socketAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);

	return std::string(ipBuffer);
}


int Socket::getPeerPort(void) const {
	struct sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(socketAddress);
	std::memset(&socketAddress, 0x00, sizeof(socketAddress));

	int result = ::getpeername(socket, (struct sockaddr *) &socketAddress, &socketAddressLength);
	if (result < 0) {
		LOG(ERROR) << "Impossible to obtain socket information for getting peer port, cause '" << errno << "'.";
		return -1;
	}

	return ntohs(socketAddress.sin_port);
}


int Socket::getSocketDescriptor(void) const {
	return socket;
}


bool Socket::isClosed(void) const {
	socklen_t valueLength = 0;

	int result = getsockopt(socket, SOL_SOCKET, SO_TYPE, &result, &valueLength);
	if ( (result < 0) && (errno == EBADF) ) {
		return true;
	}

	return false;
}


void Socket::setNonBlocking(const bool pEnable) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags < 0) {
		LOG(ERROR) << "Impossible to get socket flags to enable or disable non-blocking state.";
		return;
	}

	flags = pEnable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	if (fcntl(socket, F_SETFL, flags) < 0) {
		LOG(ERROR) << "Impossible to set non-blocking socket state.";
	}
}


void Socket::setReceiveTimeout(const time_t pSeconds, const suseconds_t pMicroseconds) {
	struct timeval timeout;
	timeout.tv_sec = pSeconds;
	timeout.tv_usec = pMicroseconds;

	if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		LOG(ERROR) << "Impossible to set timeout for receiving, cause '" << errno << "'.";
	}
}


void Socket::setAddressReusage(const bool pEnable) {
	int optionValue = pEnable ? 1 : 0;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0) {
		LOG(ERROR) << "Impossible to enable or disable address reusage.";
	}
}


Socket & Socket::operator=(Socket && pOther) {
	if (this != &pOther) {
		socket = pOther.socket;
		pOther.socket = -1;
	}

	return *this;
}
