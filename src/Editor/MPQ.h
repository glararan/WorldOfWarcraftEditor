#ifndef MPQ_H
#define MPQ_H

#include "lib_mpq.h"
#include "mpq-internal.h"

#include <string.h>
#include <ctype.h>
#include <vector>
#include <iostream>
#include <deque>

using namespace std;

class MPQArchive
{

public:
	mpq_archive *mpq_a;

	MPQArchive(const char* filename);
	void close();
};

typedef deque<MPQArchive*> ArchiveSet;

class MPQFile
{
	char fname[1024];
	//MPQHANDLE handle;
	bool eof;
	char *buffer;
	libmpq__off_t pointer, size;

	bool External;

	// disable copying
	MPQFile(const MPQFile &f) {}
	void operator = (const MPQFile &f) {}

public:
	MPQFile(const char* filename);	// filenames are not case sensitive
	MPQFile(const char* filename, bool Fake);
	~MPQFile();
	size_t read(void* dest, size_t bytes);
	size_t getSize() { return size; }
	size_t getPos() { return pointer; }
	char* getBuffer() { return buffer; }
	char* getPointer() { return buffer + pointer; }
	bool isEof() { return eof; }
	void seek(int offset);
	void seekRelative(int offset);
	void close();
	bool isExternal(){return External;};
	
	void setBuffer(char *Buf, unsigned int Size)
	{
		if(buffer)
			delete buffer;
		buffer = Buf;
		size = Size;
	};
	void SaveFile();
};

inline void flipcc(char *fcc)
{
	char t;
	t = fcc[0];
	fcc[0] = fcc[3];
	fcc[3] = t;
	t = fcc[1];
	fcc[1] = fcc[2];
	fcc[2] = t;
}

#endif