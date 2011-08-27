#ifndef UTIL_H
#define UTIL_H

#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
	#endif
	#include <hash_set>
#else
	#include <ext/hash_set>
#endif

// include opengl stuff
//#include "openglheaders.h"

// STL headers
#include <algorithm>
//#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <vector>

// Standard C++ headers
#include <stdio.h>

// wx
/*#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/string.h>
#include <wx/log.h>*/

// Our other utility headers
#include "Vec3D.h"

using namespace std;

float frand();
float randfloat(float lower, float upper);
int randint(int lower, int upper);

template <class T>
bool from_string(T& t, const string& s, ios_base& (*f)(ios_base&))
{
  istringstream iss(s);
  return !(iss >> f >> t).fail();
}



#endif