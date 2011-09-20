#include "DBCFile.h"
#include "MPQ.h"
#include "OpenGL.h"
#undef min
#undef max

#include <cstdio>

DBCFile::DBCFile(const string &filename) : filename(filename), data(0)
{
	
}

bool DBCFile::open()
{
	gLog("[World of Warcraft Studio - Editor] - Opening DBC : %s\n", filename.c_str());
    MPQFile f(filename.c_str());

    char header[4];
    unsigned int na, nb, es, ss;

    if(f.read(header, 4) != 4)
		return false;

    if(header[0] != 'W' || header[1] != 'D' || header[2] != 'B' || header[3] != 'C')
    {
		f.close();
        gLog("Critical Error: An error occured while trying to read the DBCFile : %s.", filename.c_str());
        return false;
    }

	// Number of records      Number of fields       Size of a record       String size
    if(f.read(&na, 4) != 4 && f.read(&nb, 4) != 4 && f.read(&es, 4) != 4 && f.read(&ss, 4) != 4)
		return false;

    recordSize = es;
    recordCount = na;
    fieldCount = nb;
    stringSize = ss;

	gLog("Test");
    
	if(fieldCount * 4 != recordSize)
		return false;

    data = new unsigned char[recordSize * recordCount + stringSize];
    stringTable = data + recordSize * recordCount;

	size_t data_size = recordSize * recordCount + stringSize;
    if(f.read(data, data_size) != data_size)
		return false;

    f.close();
	gLog("[World of Warcraft Studio - Editor] - Closed & Read in DBC : %s\n", filename.c_str());

    return true;
}
DBCFile::~DBCFile()
{
	delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
	assert(data);
	return Record(*this, data + id * recordSize);
}

DBCFile::Iterator DBCFile::begin()
{
	assert(data);
	return Iterator(*this, data);
}
DBCFile::Iterator DBCFile::end()
{
	assert(data);
	return Iterator(*this, stringTable);
}