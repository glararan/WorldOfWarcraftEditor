#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>
#include <map>

#include "Chameleon.h"

using namespace std;

class ConfigFile
{
  map<string, Chameleon> content_;

public:
  ConfigFile(string const& configFile);

  Chameleon const& Value(string const& section, string const& entry) const;

  Chameleon const& Value(string const& section, string const& entry, double value);
  Chameleon const& Value(string const& section, string const& entry, string const& value);
};

#endif