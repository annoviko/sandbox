#ifndef SOCKET_HPP_
#define SOCKET_HPP_


#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <unistd.h>
#include <sys/types.h>


class Socket {
public:
	enum class Protocol {
		UDP = 1,
		TCP = 2
	};

protected:
	int		socket;

public:
	Socket(void);

	explicit Socket(const Protocol);

	explicit Socket(const int pStyle, const int pProtocol);

	explicit Socket(const int pSocketDescriptor);

	Socket(const Socket & pSocket) = delete;

	Socket(Socket && pSocket);

	virtual ~Socket(void);

public:
	int bind(const std::string & pAddress, const int pPort);

	int connect(const std::string & pAddress, const int pPort);

	void close(void);

	virtual void send(const std::string & pMessage) const;

	virtual void send(const unsigned char * const pMessage, const size_t pLength) const;

	virtual void sendTo(const std::string & pMessage, const std::string & pAddress, const int pPort) const;

	virtual void sendTo(const unsigned char * const pMessage, const size_t pLength, const std::string & pAddress, const int pPort) const;

	virtual ssize_t receive(unsigned char * pMessage, const size_t pLength) const;

	virtual ssize_t receiveFrom(unsigned char * pMessage, const size_t pLength, std::string & pAddress, int & pPort) const;

	int accept(Socket & pTransport);
	
	int listen(const int pAmountConnections);

public:
	std::string getAddress(void) const;

	int getPort(void) const;

	std::string getPeerAddress(void) const;

	int getPeerPort(void) const;

	int getSocketDescriptor(void) const;

	bool isClosed(void) const;

	void setNonBlocking(const bool pEnable);

	void setReceiveTimeout(const time_t pSeconds, const suseconds_t pMicroseconds);

	void setAddressReusage(const bool pEnable);

private:
	void createSocket(const Protocol);
	void createSocket(const int pStyle = SOCK_STREAM, const int pProtocol = IPPROTO_TCP);

public:
	Socket & operator=(const Socket & pOther) = delete;

	Socket & operator=(Socket && pOther);
};


#endif
