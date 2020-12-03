#include "XmlMessageBuilder.hpp"


XmlMessageBuilder::XmlMessageBuilder(const std::string & pMessageRootNode) {
	xmlDocument = xmlNewDoc(BAD_CAST "1.0");
	xmlNodeRoot = xmlNewNode(NULL, BAD_CAST pMessageRootNode.c_str());
	xmlDocSetRootElement(xmlDocument, xmlNodeRoot);
}


XmlMessageBuilder::~XmlMessageBuilder(void) {
	if (xmlDocument != nullptr) {
		xmlFreeDoc(xmlDocument);
	}
}


void XmlMessageBuilder::addNode(const std::string & pName, const std::string & pValue) {
	xmlNewChild(xmlNodeRoot, NULL, BAD_CAST pName.c_str(), BAD_CAST pValue.c_str());
}


std::string XmlMessageBuilder::toString(void) {
	std::string result = "";

	if (xmlDocument != nullptr) {
		xmlChar * xmlMessageBuffer;
		int xmlMessageBufferSize;

		xmlDocDumpFormatMemory(xmlDocument, &xmlMessageBuffer, &xmlMessageBufferSize, 1);
		result = std::string((char *) xmlMessageBuffer, xmlMessageBufferSize);

		xmlFree(xmlMessageBuffer);
	}

	return result;
}
