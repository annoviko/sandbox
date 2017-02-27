#ifndef XMLMESSAGEPARSER_HPP_
#define XMLMESSAGEPARSER_HPP_


#include <string>
#include <libxml/parser.h>


class XmlMessageParser {
private:
	xmlDocPtr	xmlDocument		= nullptr;

public:
	XmlMessageParser(void) = default;

	XmlMessageParser(const std::string & pMessage);

	~XmlMessageParser(void);

public:
	std::string getValue(const std::string & pNodePath);
};


#endif
