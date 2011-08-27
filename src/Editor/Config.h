#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

#include "Chameleon\Chameleon.h"

class Config {
	std::map<std::string,Chameleon> content_;

public:
	Config(std::string const& config);

	Chameleon const& Value(std::string const& section, std::string const& entry) const;

	Chameleon const& Value(std::string const& section, std::string const& entry, double value);
	Chameleon const& Value(std::string const& section, std::string const& entry, std::string const& value);
};

#endif