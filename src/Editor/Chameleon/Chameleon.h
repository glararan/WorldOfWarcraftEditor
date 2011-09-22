#ifndef CHAMELEON_H
#define CHAMELEON_H

#include <string>

using namespace std;

class Chameleon
{
public:
    Chameleon() {};
    explicit Chameleon(const string&);
    explicit Chameleon(double);
    explicit Chameleon(const char*);

    Chameleon(const Chameleon&);
    Chameleon& operator=(Chameleon const&);

    Chameleon& operator=(double);
    Chameleon& operator=(string const&);

public:
    operator string() const;
    operator double     () const;
private:
    string value_;
};

#endif