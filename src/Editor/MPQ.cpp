#include "MPQ.h"
#include "OpenGL.h"
#include "Directory.h"

#include <cstdio>
#include <deque>

ArchiveSet gOpenArchives;

extern char wowpath[1024];

MPQArchive::MPQArchive(const char* filename)
{
	int result = libmpq__archive_open(&mpq_a, filename, -1);
	gLog("[World of Warcraft Studio - Editor] - Opening %s\n", filename);
	if(result)
	{
        switch(result) {
            case LIBMPQ_ERROR_OPEN:
                gLog("[World of Warcraft Studio - Editor] - Error opening archive '%s': Does file really exist?\n", filename);
                break;

            case LIBMPQ_ERROR_FORMAT:  /* bad file format */
                gLog("[World of Warcraft Studio - Editor] - Error opening archive '%s': Bad file format\n", filename);
                break;

            case LIBMPQ_ERROR_SEEK:   /* seeking in file failed */
                gLog("[World of Warcraft Studio - Editor] - Error opening archive '%s': Seeking in file failed\n", filename);
                break;

            case LIBMPQ_ERROR_READ:   /* Read error in archive */
                gLog("[World of Warcraft Studio - Editor] - Error opening archive '%s': Read error in archive\n", filename);
                break;

            case LIBMPQ_ERROR_MALLOC: /* maybe not enough memory? :) */
                gLog("[World of Warcraft Studio - Editor] - Error opening archive '%s': Maybe not enough memory\n", filename);
                break;

            default:
				gLog("[World of Warcraft Studio - Editor] - Error opening archive %s\n", filename);
				break;
		}
		return;
	}
	gOpenArchives.push_front(this);
}

void MPQArchive::close()
{
	//gOpenArchives.erase(erase(&mpq_a);
	libmpq__archive_close(mpq_a);
}

MPQFile::MPQFile(const char* filename):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	char Temp[1024];

	gLog("[World of Warcraft Studio - Editor] - Attempting to Load File %s\n",filename);
	sprintf(Temp,"%s\\%s",wowpath,filename);
	strcpy(fname,Temp);
	FILE* fd;
	fd = fopen(Temp,"rb");

	if(fd!=NULL)
	{
		gLog("[World of Warcraft Studio - Editor] - File %s found in root directory\n",filename);
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
		gLog("[World of Warcraft Studio - Editor] - File %s found in data directory\n",filename);
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
		mpq_archive *mpq_a = (*i)->mpq_a;
		uint32_t fileno;
		if(libmpq__file_number(mpq_a, filename, &fileno))
			continue;

		libmpq__off_t transferred;
        libmpq__file_unpacked_size(mpq_a, fileno, &size);

		gLog("[World of Warcraft Studio - Editor] - File %s found inside %s\n", filename, (char)filename[259]); 

		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size<=1)
		{
			gLog("[World of Warcraft Studio - Editor] - Invalid File of size 1 for file %s (damn Blizzard :-p)\n",filename);
			eof = true;
			buffer = 0;
			return;
		}
		buffer = new char[size];

		//libmpq__file_getdata(&mpq_a, fileno, (unsigned char*)buffer);
		libmpq__file_read(mpq_a, fileno, (unsigned char*)buffer, size, &transferred);
		External=false;
		return;
	}
	gLog("[World of Warcraft Studio - Editor] - Unable to find file %s\n",filename);
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
		mpq_archive *mpq_a = (*i)->mpq_a;
		uint32_t fileno;
		if(libmpq__file_number(mpq_a, filename, &fileno))
			continue;
		libmpq__off_t transferred;
        libmpq__file_unpacked_size(mpq_a, fileno, &size);
		// Found!
		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size<=1)
		{
			eof = true;
			buffer = 0;
			return;
		}
		buffer = new char[size];
		//libmpq__file_getdata(&mpq_a, fileno, (unsigned char*)buffer);
		libmpq__file_read(mpq_a, fileno, (unsigned char*)buffer, size, &transferred);
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
	if (rpos > size)
	{
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
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

void MPQFile::SaveFile()
{
	FILE* fd;

	CreatePath(fname);

	fd = fopen(fname,"wb");

	if(fd!=NULL)
	{
		gLog("[World of Warcraft Studio - Editor] - Saving File %s\n",fname);
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
	if (succ)
	{
		MPQARCHIVE *ar = (MPQARCHIVE*)(handle);
        succ = true;
	}
	else
	{
		gLog("[World of Warcraft Studio - Editor] - Error opening archive %s\n", filename);
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
	if (succ)
	{
		DWORD s = SFileGetFileSize(handle, 0);
		buffer = new char[s];
		SFileReadFile(handle, buffer, s, 0, 0);
		SFileCloseFile(handle);
		size = (size_t)s;
		eof = false;
	}
	else
	{
		eof = true;
		buffer = 0;
	}
}

size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size)
	{
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
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

#endif