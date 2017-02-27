#ifndef LINKENCAP_HPP_
#define LINKENCAP_HPP_


#include <string>


class LinkEncap {
protected:
	std::string devLink     = "";
	std::string ipLink      = "";

public:
	LinkEncap(void) = default;

	LinkEncap(const LinkEncap & pOther) = default;

	LinkEncap(LinkEncap && pOther) = default;

	virtual ~LinkEncap(void) = default;

public:
	void open(const std::string & pDevLink);

	void route(const std::string & pDestAddress) const;

	void route(const std::string & pDestAddress, const std::string & pGateway) const;

	void unroute(const std::string & pDestAddress) const;

	void unroute(const std::string & pDestAddress, const std::string & pGateway) const;

	bool exist(void) const;

	static bool exist(const std::string & pDevLink);

public:
	bool hasRoute(const std::string & pDestAddress) const;

	std::string getIp(void) const;

	std::string getDevId(void) const;
};


#endif
