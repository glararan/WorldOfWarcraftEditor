#ifndef AREADB_H
#define AREADB_H

#include "DBCFile.h"

class AreaDB: public DBCFile
{
public:
	AreaDB();
	~AreaDB();

	/// Fields
	static const size_t AreaID = 0;		// uint
	static const size_t Continent = 1;	// uint
	static const size_t Region = 2;		// uint [AreaID]
	static const size_t Flags = 3;		// bit field
	static const size_t Name = 11;		// string
	//   00111000 City (Undercity, Ironforge, ...)
	//   10000000 Arena

	Record getByAreaID(unsigned int id);
private:
};

#endif