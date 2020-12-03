#include "LinkEncap.hpp"

#include "Logging.hpp"
#include "SystemExecutor.hpp"
#include "LinuxCommandExecutor.hpp"


void LinkEncap::open(const std::string & pDevLink) {
	if (!exist(pDevLink)) {
		LOG(ERROR) << "Specified link encap '" << pDevLink << "' is not found.";
		return;
	}

	devLink = pDevLink;

	ipLink = LinuxCommandExecutor::getIpForDevice(pDevLink);

	if (ipLink.empty()) {
		LOG(ERROR) << "Impossible to find IP address of specified link encap '" << pDevLink << "'.";
	}
	else {		
		LOG(INFO) << "Link Encap '" << devLink << "' with address '" << ipLink << "'.";
	}
}


void LinkEncap::route(const std::string & pDestAddress) const {
	route(pDestAddress, "");
}


void LinkEncap::route(const std::string & pDestAddress, const std::string & pGateway) const {
	LinuxCommandExecutor::ipRouteAdd(pDestAddress, pGateway, devLink);
}


void LinkEncap::unroute(const std::string & pDestAddress) const {
	LinuxCommandExecutor::ipRouteDel(pDestAddress);
}


void LinkEncap::unroute(const std::string & pDestAddress, const std::string & pGateway) const {
	LinuxCommandExecutor::ipRouteDel(pDestAddress);
}


bool LinkEncap::exist(void) const {
	return exist(devLink);
}


bool LinkEncap::exist(const std::string & pDevLink) {
	if (pDevLink.empty()) {
		return false;
	}
	return LinuxCommandExecutor::checkIfcExist(pDevLink);
}


bool LinkEncap::hasRoute(const std::string & pDestAddress) const {
	return LinuxCommandExecutor::checkRouteExist(pDestAddress);
}


std::string LinkEncap::getIp(void) const {
	return ipLink;
}


std::string LinkEncap::getDevId(void) const {
	return devLink;
}

