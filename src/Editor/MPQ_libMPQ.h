#ifndef MPQ_H
#define MPQ_H

//#include "SFmpqapi.h"
#include "libmpq/MPQ.h"

class MPQArchive
{
	//MPQHANDLE handle;
	mpq_archive mpq_a;
public:
	MPQArchive(const char* filename);
	void close();
};

class MPQFile
{
	char fname[1024];
	//MPQHANDLE handle;
	bool eof;
	char *buffer;
	size_t pointer,size;

	bool External;

	// disable copying
	MPQFile(const MPQFile &f) {}
	void operator=(const MPQFile &f) {}

public:
	MPQFile(const char* filename);	// filenames are not case sensitive
	MPQFile(const char* filename, bool Fake);
	~MPQFile();
	size_t read(void* dest, size_t bytes);
	size_t getSize();
	size_t getPos();
	char* getBuffer();
	char* getPointer();
	bool isEof();
	void seek(int offset);
	void seekRelative(int offset);
	void close();
	bool isExternal(){return External;};
	
	void setBuffer(char *Buf, unsigned int Size)
	{
		if(buffer)
			delete buffer;
		buffer=Buf;
		size=Size;
	};
	void SaveFile();
};

inline void flipcc(char *fcc)
{
	char t;
	t=fcc[0];
	fcc[0]=fcc[3];
	fcc[3]=t;
	t=fcc[1];
	fcc[1]=fcc[2];
	fcc[2]=t;
}

#endif