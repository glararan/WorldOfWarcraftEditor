#include "AreaDB.h"
#include "DBCFile.h"

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

void InitAreaDB()
{
	AreaTable area;
	DBCFile AreaDB("DBFilesClient\\AreaTable.dbc");
	
	for(DBCFile::Iterator i = AreaDB.begin(); i != AreaDB.end(); ++i)
	{
		area.ID = i->getUInt(0);
		area.mapid = i->getUInt(1);
		area.zone = i->getUInt(2);
		area.exploreFlag = i->getUInt(3);
		area.flags = i->getUInt(4);
		area.area_level = i->getInt(5);
		area.area_name[0] = i->getString(6);
		area.team = i->getUInt(7);
	}
}