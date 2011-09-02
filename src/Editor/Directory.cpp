#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <shlobj.h> 

void ConvertStringToWChar (LPCTSTR str, WCHAR *buffer)
{
	int i;
       for (int i=0; str[i]; i++)
             buffer[i] = (WCHAR)str[i];
       buffer[i] = 0;
}

void CreatePath(const char *fname)
{
	int		i;
	char	path[MAX_PATH];
	WCHAR	buffer[MAX_PATH];
	
	strcpy(path,fname);

	i=strlen(path);

	while(path[i]!='\\'&&i>0)
		i--;
	path[i]=0;

	ConvertStringToWChar(path,buffer);
	SHCreateDirectory(NULL,buffer);
}