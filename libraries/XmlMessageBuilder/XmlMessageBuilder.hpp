#ifndef XMLMESSAGEBUILDER_HPP_
#define XMLMESSAGEBUILDER_HPP_


#include <libxml/parser.h>
#include <string>


class XmlMessageBuilder {
private:
	xmlDocPtr	xmlDocument		= nullptr;
	xmlNodePtr	xmlNodeRoot		= nullptr;

public:
	XmlMessageBuilder(void) = default;

	XmlMessageBuilder(const std::string & pMessageRootNode);

	~XmlMessageBuilder(void);

public:
	void addNode(const std::string & pName, const std::string & pValue);

	std::string toString(void);
};


#endif
