#ifndef _GRETUNNEL_HPP_
#define _GRETUNNEL_HPP_


#include <string>

#include "LinkEncap.hpp"


class GreTunnel : public LinkEncap {
public:
    GreTunnel(void);

    virtual ~GreTunnel(void);

public:
    void open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pIpRemote);
    void open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pIpRemote, const std::string & pKey, const std::string & pDev);
    void close(void);

public:
    const std::string & getKey(void) const;
    const std::string & getIpLocal(void) const;
    const std::string & getIpRemote(void) const;
    const std::string & getDevIdLocal(void) const;

private:
	std::string key;
	std::string ipLocal;
	std::string ipRemote;
	std::string dev;
};


#endif
