#define _CRT_SECURE_NO_WARNINGS
#include "MPQ_libMPQ.h"
#include "OpenGL.h"
#include "Directory.h"

#include <vector>
#include <cstdio>
#include <cstring>


typedef std::vector<mpq_archive*> ArchiveSet;
ArchiveSet gOpenArchives;

extern char wowpath[1024];

MPQArchive::MPQArchive(const char* filename)
{
	int result = libmpq_archive_open(&mpq_a, (unsigned char*)filename);
	gLog("Opening %s\n", filename);
	if(result) {
		gLog("Error opening archive %s\n", filename);
		return;
	}
	gOpenArchives.push_back(&mpq_a);
}

void MPQArchive::close()
{
	//gOpenArchives.erase(erase(&mpq_a);
	libmpq_archive_close(&mpq_a);
}

MPQFile::MPQFile(const char* filename):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	char Temp[1024];

	gLog("Attempting to Load File %s\n",filename);
	sprintf(Temp,"%s\\%s",wowpath,filename);
	strcpy(fname,Temp);
	FILE* fd;
	fd = fopen(Temp,"rb");

	if(fd!=NULL)
	{
		gLog("File %s found in root directory\n",filename);
		fseek(fd,0,SEEK_END);
		size=ftell(fd);
		
		buffer = new char[size];
		fseek(fd,0,SEEK_SET);
		fread(buffer,1,size,fd);
		fclose(fd);
		External=true;
		return;
	}

	sprintf(Temp,"%s\\data\\%s",wowpath,filename);
	fd = fopen(Temp,"rb");

	if(fd!=NULL)
	{
		gLog("File %s found in data directory\n",filename);
		fseek(fd,0,SEEK_END);
		size=ftell(fd);
		
		buffer = new char[size];
		fseek(fd,0,SEEK_SET);
		fread(buffer,1,size,fd);
		fclose(fd);
		External=true;
		return;
	}

	for(ArchiveSet::iterator i=gOpenArchives.begin(); i!=gOpenArchives.end();++i)
	{
		
		mpq_archive &mpq_a = **i;
		int fileno = libmpq_file_number(&mpq_a, filename);
		if(fileno == LIBMPQ_EFILE_NOT_FOUND)
		{
			//gLog("\tFile %s not found inside %s\n",filename,mpq_a.filename); 
			continue;
		}
		gLog("File %s found inside %s\n",filename,mpq_a.filename); 
		// Found!
		size = libmpq_file_info(&mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fileno);
		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size<=1) {
			gLog("Invalid File of size 1 for file %s (damn Blizzard :-p)\n",filename);
			eof = true;
			buffer = 0;
			return;
		}
		buffer = new char[size];
		libmpq_file_getdata(&mpq_a, fileno, (unsigned char*)buffer);
		External=false;
		return;
	}
	gLog("Unable to find file %s\n",filename);
	eof = true;
	buffer = 0;
}

MPQFile::MPQFile(const char* filename, bool Fake):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	char Temp[1024];

	sprintf(Temp,"%s\\%s",wowpath,filename);
	strcpy(fname,Temp);
	for(ArchiveSet::iterator i=gOpenArchives.begin(); i!=gOpenArchives.end();++i)
	{
		mpq_archive &mpq_a = **i;
		int fileno = libmpq_file_number(&mpq_a, filename);
		if(fileno == LIBMPQ_EFILE_NOT_FOUND)
			continue;
		// Found!
		size = libmpq_file_info(&mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fileno);
		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size<=1) {
			eof = true;
			buffer = 0;
			return;
		}
		buffer = new char[size];
		libmpq_file_getdata(&mpq_a, fileno, (unsigned char*)buffer);
		External=false;
		return;
	}
	eof = true;
	buffer = 0;
}

MPQFile::~MPQFile()
{
	close();
}


size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size) {
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
}

bool MPQFile::isEof()
{
    return eof;
}

void MPQFile::seek(int offset)
{
	pointer = offset;
	eof = (pointer >= size);
}

void MPQFile::seekRelative(int offset)
{
	pointer += offset;
	eof = (pointer >= size);
}

void MPQFile::close()
{
	if (buffer) delete[] buffer;
	buffer = 0;
	eof = true;
}

size_t MPQFile::getSize()
{
	return size;
}

size_t MPQFile::getPos()
{
	return pointer;
}

char* MPQFile::getBuffer()
{
	return buffer;
}

char* MPQFile::getPointer()
{
	return buffer + pointer;
}


void MPQFile::SaveFile()
{
	FILE* fd;

	CreatePath(fname);

	fd = fopen(fname,"wb");

	if(fd!=NULL)
	{
		gLog("  Saving File %s\n",fname);
		fseek(fd,0,SEEK_SET);
		fwrite(buffer,1,size,fd);
		fclose(fd);
		External=true;
		return;
	}
}

#if 0
int _id=1;

MPQArchive::MPQArchive(const char* filename)
{
	BOOL succ = SFileOpenArchive(filename, _id++, 0, &handle);
	if (succ) {
		MPQARCHIVE *ar = (MPQARCHIVE*)(handle);
        succ = true;
	} else {
		gLog("Error opening archive %s\n", filename);
	}
}

void MPQArchive::close()
{
	SFileCloseArchive(handle);
}

MPQFile::MPQFile(const char* filename)
{
	BOOL succ = SFileOpenFile(filename, &handle);
	pointer = 0;
	if (succ) {
		DWORD s = SFileGetFileSize(handle, 0);
		buffer = new char[s];
		SFileReadFile(handle, buffer, s, 0, 0);
		SFileCloseFile(handle);
		size = (size_t)s;
		eof = false;
	} else {
		eof = true;
		buffer = 0;
	}
}

size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size) {
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
}

bool MPQFile::isEof()
{
    return eof;
}

void MPQFile::seek(int offset)
{
	pointer = offset;
	eof = (pointer >= size);
}

void MPQFile::seekRelative(int offset)
{
	pointer += offset;
	eof = (pointer >= size);
}

void MPQFile::close()
{
	if (buffer) delete[] buffer;
	buffer = 0;
	eof = true;
}

size_t MPQFile::getSize()
{
	return size;
}

size_t MPQFile::getPos()
{
	return pointer;
}

char* MPQFile::getBuffer()
{
	return buffer;
}

char* MPQFile::getPointer()
{
	return buffer + pointer;
}


#endif