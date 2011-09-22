#include <sstream>

#include "Chameleon.h"

Chameleon::Chameleon(string const& value)
{
    value_ = value;
}

#include <iostream>

Chameleon::Chameleon(const char* c)
{
    value_ = c;
}

Chameleon::Chameleon(double d)
{
    stringstream s;
    s << d;
    value_ = s.str();
}

Chameleon::Chameleon(Chameleon const& other)
{
    value_ = other.value_;
}

Chameleon& Chameleon::operator=(Chameleon const& other)
{
    value_ = other.value_;
    return *this;
}

Chameleon& Chameleon::operator=(double i)
{
    stringstream s;
    s << i;
    value_ = s.str();
    return *this;
}

Chameleon& Chameleon::operator=(std::string const& s)
{
    value_ = s;
    return *this;
}

Chameleon::operator string() const
{
    return value_;
}

Chameleon::operator double() const
{
    return atof(value_.c_str());
}