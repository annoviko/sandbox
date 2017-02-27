#include "XmlMessageParser.hpp"

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <cstring>

#include "Logging.hpp"


XmlMessageParser::XmlMessageParser(const std::string & pMessage) {
	xmlDocument = xmlParseDoc(BAD_CAST pMessage.c_str());
	if (xmlDocument == nullptr) {
		LOG(ERROR) << "Impossible to parse XML message '" << pMessage << "'.";
	}
}


XmlMessageParser::~XmlMessageParser(void) {
	if (xmlDocument != nullptr) {
		xmlFreeDoc(xmlDocument);
	}
}


std::string XmlMessageParser::getValue(const std::string & pNodePath) {
	std::string result = "";

	if (xmlDocument == nullptr) {
		LOG(ERROR) << "XML document is not initialized.";
		return result;
	}

	xmlXPathContextPtr xPathContext = xmlXPathNewContext(xmlDocument);
	xmlXPathObjectPtr xPathObject = xmlXPathEvalExpression(BAD_CAST pNodePath.c_str(), xPathContext);

	if (xPathObject == nullptr) {
		LOG(ERROR) << "Impossible to find XML path '" << pNodePath << "'.";

		xmlXPathFreeContext(xPathContext);

		return "";
	}

	xmlNodeSetPtr nodes = xPathObject->nodesetval;
	const int amountNodes = nodes ? nodes->nodeNr : 0;

	if (amountNodes > 1) {
		LOG(WARNING) << "There are several nodes on current path '" << pNodePath << "'.";
	}

	if (amountNodes != 0) {
		xmlNodePtr node = nodes->nodeTab[0];
		xmlChar * value = xmlNodeGetContent(node);
		if (value != nullptr) {
			result = std::string((char *) value);
			xmlFree(value);
		}
		else {
			LOG(WARNING) << "Impossible to get content of XML node using path '" << pNodePath << "'.";
		}
	}
	else {
		LOG(WARNING) << "Impossible to find XML node using path '" << pNodePath << "'.";
	}

	xmlXPathFreeObject(xPathObject);
	xmlXPathFreeContext(xPathContext);

	return result;
}
