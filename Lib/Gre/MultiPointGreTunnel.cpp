#include "MultiPointGreTunnel.hpp"

#include "Logging.hpp"
#include "SystemExecutor.hpp"
#include "LinuxCommandExecutor.hpp"
#include "Utils.hpp"


MultiPointGreTunnel::MultiPointGreTunnel(void)
{ }


MultiPointGreTunnel::~MultiPointGreTunnel(void)
{ }


void MultiPointGreTunnel::open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal) {
    open(pDevTunnel, pIpTunnel, pIpLocal, "", "", "");
}


void MultiPointGreTunnel::open(const std::string & pDevTunnel, const std::string & pIpTunnel, const std::string & pIpLocal, const std::string & pKey, const std::string & pDev) {
    LOG(INFO) << "Create GRE Tunnel in Multi-Point mode.";
    GreTunnel::open(pDevTunnel, pIpTunnel, pIpLocal, "", pKey, pDev);
}


void MultiPointGreTunnel::add(const std::string & pIpRemoteTunnel, const std::string & pIpRemote) {
    if (exist()) {
		LinuxCommandExecutor::ipNeighAdd(pIpRemoteTunnel, pIpRemote, devLink);
    }
}


void MultiPointGreTunnel::remove(const std::string & pIpRemoteTunnel) {
    if (exist()) {
		LinuxCommandExecutor::ipNeighDel(pIpRemoteTunnel, devLink);
    }
}


void MultiPointGreTunnel::get(std::vector<std::string> & pIpAttachedTunnels) {
    pIpAttachedTunnels.clear();

    if (exist()) {
        ExecutorResult result;
        SystemExecutor::execute("ip neigh show dev " + devLink + " | grep -v FAILED | awk '{print $1;}'", result);

        std::string & textResult = result.getTextResult();
	utils::tokenize(textResult, "\n", pIpAttachedTunnels);
    }
}
