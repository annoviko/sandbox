#ifndef LINUXCOMMANDEXECUTOR_HPP_
#define LINUXCOMMANDEXECUTOR_HPP_


#include "SystemExecutor.hpp"
#include "Logging.hpp"
#include "Utils.hpp"


class LinuxCommandExecutor {
private:
	LinuxCommandExecutor() = delete;

	LinuxCommandExecutor(const LinuxCommandExecutor & pOther) = delete;

	LinuxCommandExecutor(LinuxCommandExecutor && pOther) = delete;

	~LinuxCommandExecutor() = delete;

public:
	static int ipTunnelAdd(const std::string & tunnelName, const std::string & ipLocal,
		const std::string & ipRemote, const std::string & key, const std::string & dev) {
		std::string remoteInfo = (!ipRemote.empty())?(" remote " + ipRemote):"";
		std::string devInfo = (!dev.empty())?(" dev " + dev):"";
		std::string keyInfo = (!key.empty())?" key " + key:"";
		return SystemExecutor::execute("ip tunnel add " + tunnelName + " mode gre local " + ipLocal + remoteInfo + " ttl 255" + keyInfo + devInfo);
	}

	static int ipTunnelDel(const std::string & tunnelName) {
		return SystemExecutor::execute("ip tunnel del " + tunnelName);
	}

	static int ipAddrAdd(const std::string & tunnelName, const std::string & tunnelIp) {
		return SystemExecutor::execute("ip addr add " + tunnelIp + "/24 dev " + tunnelName);
	}

	static int ipLinkSet(const std::string & tunnelName) {
		disableRpFilter(tunnelName);
		return SystemExecutor::execute("ip link set " + tunnelName + " up");
	}

	static int ipRouteAdd(const std::string & ip, const std::string & gw, const std::string & dev) {
		std::string devInfo = (!dev.empty())?(" dev " + dev):"";
		std::string gwInfo = (!gw.empty())?" gateway " + gw:"";
		return SystemExecutor::execute("ip route add  " + ip + gwInfo + devInfo);
	}

	static int ipRouteChange(const std::string & ip, const std::string & gw, const std::string & dev) {
		std::string devInfo = (!dev.empty())?(" dev " + dev):"";
		std::string gwInfo = (!gw.empty())?" gateway " + gw:"";
		return SystemExecutor::execute("ip route change  " + ip + gwInfo + devInfo);
	}

	static int ipRouteDel(const std::string & ip) {
		return SystemExecutor::execute("ip route del " + ip);
	}

	static int ipNeighAdd(const std::string & ipRemoteTunnel, const std::string & ipRemote, const std::string & dev) {
		return SystemExecutor::execute("ip neigh add " + ipRemoteTunnel + " lladdr " + ipRemote + " dev " + dev);
	}

	static int ipNeighDel(const std::string & ipRemoteTunnel, const std::string & dev) {
		return SystemExecutor::execute("ip neigh del " + ipRemoteTunnel + " dev " + dev);
	}

	static long getDlBytes(const std::string & pDevLink) {
		ExecutorResult bytes;
		SystemExecutor::execute("cat /sys/class/net/" + pDevLink + "/statistics/tx_bytes", bytes);
		return utils::STOL(bytes.getTextResult());
	}

	static long getUlBytes(const std::string & pDevLink) {
		ExecutorResult bytes;
		SystemExecutor::execute("cat /sys/class/net/" + pDevLink + "/statistics/rx_bytes", bytes);
		return utils::STOL(bytes.getTextResult());
	}

	static std::string getIpForDevice(const std::string & pDevLink) {
		ExecutorResult result;
		SystemExecutor::execute("ifconfig -a " + pDevLink + " | grep \"inet addr\" | awk '{print $2}' | grep -E -o \"([0-9]{1,3}[\\.]){3}[0-9]{1,3}\"", result);
		return result.getTextResult();
	}
	
	static bool checkIfcExist(const std::string & pDevLink) {
		ExecutorResult result;
		SystemExecutor::execute("ifconfig -s | grep \"" + pDevLink + "\"", result);
		return (0 == result.getExitCode());
	}

	static bool checkRouteExist(const std::string & pDestAddress) {
		ExecutorResult result;
		SystemExecutor::execute("ip route show | grep \"" + pDestAddress + "\"", result);
		return (0 == result.getExitCode());
}

private:
	static int disableRpFilter(const std::string & tunnelName) {
		return SystemExecutor::execute("sysctl -w net.ipv4.conf." + tunnelName + ".rp_filter=0");
	}
};

#endif
