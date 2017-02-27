#include "Properties.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "Logging.hpp"
#include "Utils.hpp"


void Properties::load(const std::string & pFileName) {
	std::ifstream input_filestream;
	input_filestream.open(pFileName, std::ifstream::in);
	if (!input_filestream.good()) {
		LOG(WARNING) << "Impossible to read parameters from file '" << pFileName << "'";
		return;
	}

	properties.clear();

	std::string line;
	while(std::getline(input_filestream, line)) {
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

		std::string key, value;
		if (parse(line, key, value)) {
			properties.insert({key, value});
		}
	}

	input_filestream.close();
}


std::string Properties::getProperty(const std::string & pKey) {
	return getProperty(pKey, "");
}


std::string Properties::getProperty(const std::string & pKey, const std::string & pDefaultValue) {
	std::unordered_map<std::string, std::string>::const_iterator property = properties.find(pKey);
	if (property != properties.end()) {
		return property->second;
	}

	return pDefaultValue;
}

int Properties::getIntProperty(const std::string & pKey) {
	std::string strProperty = getProperty(pKey);
	int intProperty;

	try {
		intProperty = std::stoi(strProperty);
	}
	catch (...) {
		LOG(FATAL) << "Property is found (key '" + pKey + "').\n";
		std::exit(-1);
	}

	return intProperty;
}


bool Properties::parse(const std::string & pParameter, std::string & pKey, std::string & pValue) {
	if ( (pParameter.empty()) || (pParameter.at(0) == '#') ) {
		return false;
	}

	std::stringstream stringStream(pParameter);

	if (std::getline(stringStream, pKey, '=')) {
		std::getline(stringStream, pValue);

		return true;
	}

	return false;
}
