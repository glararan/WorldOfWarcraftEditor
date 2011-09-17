#include "AreaDB.h"

AreaDB::AreaDB():
	DBCFile("DBFilesClient\\AreaTable.dbc")
{
}
AreaDB::~AreaDB()
{
}

AreaDB::Record AreaDB::getByAreaID(unsigned int id)
{
	/// Brute force search for now
	for(Iterator i = begin(); i != end(); ++i)
	{
		//return (*i);
		if(i->getUInt(AreaID) == id)
			return (*i);
	}
	throw NotFound();
}