#include "GreTunnel.hpp"

#include <algorithm>

#include "IpAddressBuilder.hpp"
#include "Logging.hpp"

//#include "MlsController.hpp"
#include "LinuxCommandExecutor.hpp"

GreTunnel::GreTunnel(void)
{ }


GreTunnel::~GreTunnel(void)
{ }


void GreTunnel::open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pIpRemote) {
    open(pDevTunnel, pIpTunnel, pIpLocal, pIpRemote, "", "");
}


void GreTunnel::open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pIpRemote, const std::string & pKey, const std::string & pDev) {
    LOG(INFO) << "Create GRE Tunnel '" << pDevTunnel << "' (" << pIpTunnel << ") '" << pIpLocal << "' -> '" << pIpRemote << "' ('" << pKey << "', '" << pDev << "')";

    devLink = pDevTunnel;
    ipLink = pIpTunnel;

	key = pKey;
	ipLocal = pIpLocal;
	ipRemote = pIpRemote;
	dev = pDev;
	
    IpAddressBuilder ipTunnelAddrBuilder(pIpTunnel);
	int rc = LinuxCommandExecutor::ipTunnelAdd(pDevTunnel, pIpLocal, pIpRemote, pKey, pDev);
    std::string subnet = ipTunnelAddrBuilder.getSubnet(0xFFFFFF00);
	
    if (0 != rc) {
        LOG(ERROR) << "Impossible to create GRE tunnel '" << pDevTunnel << "' (code: '" << rc << "').";
        return;
    }

    LinuxCommandExecutor::ipAddrAdd(pDevTunnel, pIpTunnel);
    LinuxCommandExecutor::ipLinkSet(pDevTunnel);
    LinuxCommandExecutor::ipRouteDel(subnet);
}

void GreTunnel::close(void) {
    if (exist()) {
        LinuxCommandExecutor::ipLinkSet(devLink);
		LinuxCommandExecutor::ipTunnelDel(devLink);

        devLink.clear();
        ipLink.clear();
    }
}

const std::string & GreTunnel::getKey(void) const {
    return key;
}

const std::string & GreTunnel::getIpLocal(void) const {
    return ipLocal;
}

const std::string & GreTunnel::getIpRemote(void) const {
    return ipRemote;
}

const std::string & GreTunnel::getDevIdLocal(void) const {
    return dev;
}

