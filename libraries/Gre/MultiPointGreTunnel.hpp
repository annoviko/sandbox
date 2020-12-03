#ifndef _MULTIPOINTGRETUNNEL_HPP_
#define _MULTIPOINTGRETUNNEL_HPP_


#include <vector>

#include "GreTunnel.hpp"


class MultiPointGreTunnel : public GreTunnel {
private:
    using GreTunnel::open;
    using GreTunnel::getIpRemote;

public:
    MultiPointGreTunnel(void);

    ~MultiPointGreTunnel(void);

public:
    void open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal);

    void open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pKey, const std::string & pDev);

    void add(const std::string & pIpRemoteTunnel, const std::string & pIpRemote);

    void remove(const std::string & pIpRemoteTunnel);

    void get(std::vector<std::string> & pIpAttachedTunnels);
};


#endif
