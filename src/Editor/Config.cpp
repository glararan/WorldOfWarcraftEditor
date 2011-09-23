#include "Config.h"

#include <fstream>

string trim(string const& source, char const* delims = " \t\r\n")
{
  string result(source);
  string::size_type index = result.find_last_not_of(delims);
  if(index != string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != string::npos)
    result.erase(0, index);
  else
    result.erase();

  return result;
}

ConfigFile::ConfigFile(string const& configFile)
{
  ifstream file(configFile.c_str());

  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  while(getline(file, line))
  {
    if(!line.length())
		continue;

    if(line[0] == '#')
		continue;
    if(line[0] == ';')
		continue;

    if(line[0] == '[')
	{
      inSection = trim(line.substr(1, line.find(']') - 1));
      continue;
    }

    posEqual = line.find('=');
    name  = trim(line.substr(0, posEqual));
    value = trim(line.substr(posEqual + 1));

    content_[inSection + '/' + name] = Chameleon(value);
  }
}

Chameleon const& ConfigFile::Value(string const& section, string const& entry) const
{

  map<string, Chameleon>::const_iterator ci = content_.find(section + '/' + entry);

  if(ci == content_.end())
	  throw "does not exist";

  return ci->second;
}

Chameleon const& ConfigFile::Value(string const& section, string const& entry, double value)
{
  try
  {
    return Value(section, entry);
  }
  catch(const char *)
  {
    return content_.insert(make_pair(section + '/' + entry, Chameleon(value))).first->second;
  }
}

Chameleon const& ConfigFile::Value(string const& section, string const& entry, string const& value)
{
  try
  {
    return Value(section, entry);
  }
  catch(const char *)
  {
    return content_.insert(make_pair(section + '/' + entry, Chameleon(value))).first->second;
  }
}