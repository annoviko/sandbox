#ifndef PROPERTIES_HPP_
#define PROPERTIES_HPP_


#include <string>
#include <unordered_map>


class Properties {
private:
	std::unordered_map<std::string, std::string>    properties;

public:
	Properties(void) = default;

	Properties(const Properties & pOther) = default;

	Properties(Properties && pOther) = default;

	~Properties(void) = default;

public:
	void load(const std::string & pFileName);

	std::string getProperty(const std::string & pKey);

	std::string getProperty(const std::string & pKey, const std::string & pDefaultValue);
	
	int getIntProperty(const std::string & pKey);

private:
	bool parse(const std::string & pParameter, std::string & pKey, std::string & pValue);
};


#endif
