#define NOMINMAX
#include <windows.h>
#include <winerror.h>
#include <sys\stat.h>

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>

#include "Config.h"

#include "MPQ.h"
#include "Video.h"
#include "Appstate.h"

#include "MapViewer.h"
#include "Menu.h"
#include "AreaDB.h"

#include "Shaders.h"

using namespace std;

int fullscreen = 0;

// ##################
// ## CHECK CONFIG ##
// ##################
#define CONFIG_FILE "Editor.conf"
int checkConfig()
{
	FILE* EditorPTR;

	EditorPTR = fopen(CONFIG_FILE, "r");

	if(EditorPTR) // If open Editor
	{
		gLog("[World of Warcraft Studio - Editor] - Config exist\n##############################################################\n");

		if(EditorPTR == NULL) // If conf is empty
		{
			gLog("[World of Warcraft Studio - Editor] - Config is empty\n##############################################################\n");
			exit(1);
		}
		else
			gLog("[World of Warcraft Studio - Editor] - Config have content\n##############################################################\n"); // Config isn't empty

	}
	else
	{
		gLog("[World of Warcraft Studio - Editor] - Config doesn't exist\n##############################################################\n"); // Config doesn't exist
		exit(1);
	}

	return 0;
}

// ######################
// ## EXPANSION SELECT ##
// ######################
int loadExpansion()
{
	ConfigFile conf(CONFIG_FILE);

	return (int)conf.Value("expansion_option", "Expansion");
}

// ######################
// ## GAMEPATH  SELECT ##
// ######################
char loadPath()
{
	ConfigFile conf(CONFIG_FILE);

	return (char)conf.Value("game_option", "Path");
}

// ######################
// ## GAMEVER - SELECT ##
// ######################
int loadGameVersion()
{
	ConfigFile conf(CONFIG_FILE);

	return (int)conf.Value("game_option", "GameVersion");
}

// ######################
// ## LANGUAGE  SELECT ##
// ######################
int loadLanguage()
{
	ConfigFile conf(CONFIG_FILE);

	return (int)conf.Value("language_option", "Language");
}

// ##################
// ## CHECKCONFIG2 ##
// ##################
int checkConfig2()
{
	FILE *EditorPTR;

	EditorPTR = fopen(CONFIG_FILE, "r");

	if(EditorPTR)
	{
		if(loadExpansion() != 1 && loadExpansion() != 2 && loadExpansion() != 3)
		{
			gLog("[World of Warcraft Studio - Editor] - Expansion isn't selected\n"); 
			exit(1);
		}
		
		else
			gLog("[World of Warcraft Studio - Editor] - Expansion: %d\n", loadExpansion());


		/*if(loadPath() == NULL)
			gLog("[World of Warcraft Studio - Editor] - Path is selected from Registry\n");

		else
			gLog("[World of Warcraft Studio - Editor] - Path is selected from Config - %s\n", loadPath());
		*/

		if(loadGameVersion() != 1 && loadGameVersion() != 2 && loadGameVersion() != 3 && loadGameVersion() != 4 && loadGameVersion() != 5 && loadGameVersion() != 6)
		{
			gLog("[World of Warcraft Studio - Editor] - Game Version isn't selected\n");
			exit(1);
		}
		
		else
			gLog("[World of Warcraft Studio - Editor] - GameVersion: %d\n", loadGameVersion());


		if(loadLanguage() != 1 && loadLanguage() != 2 && loadLanguage() != 3)
		{
			gLog("[World of Warcraft Studio - Editor] - Language isn't selected\n");
			exit(1);
		}
		
		else
			gLog("[World of Warcraft Studio - Editor] - Language: %d\n", loadLanguage());

	}
	return 0;
}

// ##################
// ## GRAPHIC CARD ##
// ##################
void GraphicCard()
{
	gLog("[World of Warcraft Studio - Editor] - %s\n", (char*)glGetString(GL_VENDOR));
	gLog("[World of Warcraft Studio - Editor] - %s\n", (char*)glGetString(GL_RENDERER));
	gLog("[World of Warcraft Studio - Editor] - %s\n", (char*)glGetString(GL_VERSION));
}

vector<AppState*> gStates;
bool gPop = false;

bool hasInputPath = false;
char InputPath[1024] = ".";
char path[512];
deque<MPQArchive*> archives;
extern ArchiveSet gOpenArchives;

char gamepath[1024];
char wowpath[1024];

float gFPS;

GLuint ftex;
Font *f16, *f24, *f32;
freetype::font_data arialn13, arial12, arial14, arial16, morpheus;
const char* arialtga = "Data\\Fonts\\ARIAL.TGA";
const char* arialinfo = "Data\\Fonts\\ARIAL.INFO";
const char* arialttf = "Data\\Fonts\\ARIAL.TTF";
const char* morpheusttf = "Fonts\\MORPHEUS.TTF";
const char* arialnttf = "Fonts\\ARIALN.TTF";
AreaDB gAreaDB;

void InitFonts()
{
	ftex = loadTGA(arialtga, false);

	f16 = new Font(ftex, 256, 256, 16, arialinfo);
	f24 = new Font(ftex, 256, 256, 24, arialinfo);
	f32 = new Font(ftex, 256, 256, 32, arialinfo);

	morpheus.InitMPQ(morpheusttf, 40);
	arialn13.InitMPQ(arialnttf, 13);
	
	arial12.Init(arialttf, 12);
	arial14.Init(arialttf, 14);
	arial16.Init(arialttf, 16);
}

void deleteFonts()
{
	glDeleteTextures(1, &ftex);

	delete f16;
	delete f24;
	delete f32;
}

FILE* flog;
bool glogfirst = true;

void gLog(char* str, ...)
{
	if (glogfirst)
	{
		flog = fopen("World Of Warcraft Studio - Editor.log","w");
		fclose(flog);
		glogfirst = false;
	}

	flog = fopen("World Of Warcraft Studio - Editor.log","a");

	va_list ap;

	va_start (ap, str);
	vfprintf (flog, str, ap);
	va_end (ap);

	fclose(flog);
}

void gLog_const(const char* str, ...)
{
	flog = fopen("World Of Warcraft Studio - Editor.log","a");

	va_list ap;

	va_start (ap, str);
	vfprintf (flog, str, ap);
	va_end (ap);

	fclose(flog);
}

void getGamePath()
{
#ifdef _WIN32
	HKEY key;
	DWORD t,s;
	LONG l;
	s = 1024;
	memset(gamepath,0,s);
	l = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Blizzard Entertainment\\World of Warcraft",0,KEY_QUERY_VALUE,&key);
	l = RegQueryValueEx(key,"InstallPath",0,&t,(LPBYTE)gamepath,&s);
	l = RegQueryValueEx(key,"InstallPath",0,&t,(LPBYTE)wowpath,&s);
	RegCloseKey(key);
	strcat_s(gamepath,"Data\\");
#else
	strcpy(gamepath,"data/");
#endif
}

bool scan_patches(char* scanmatch, vector<string>& pArchiveNames)
{
	int i;

	for(i = 1; i <= 99; i++)
	{
		if(i != 1)
		{
			sprintf(path, "%s-%d.MPQ", scanmatch, i);
		}
		else
		{
			sprintf(path, "%s.MPQ", scanmatch);
		}

#ifdef _WIN32
		if(FILE* h = fopen(path, "rb"))
#endif
		{
			fclose(h);
			pArchiveNames.push_back(path);
		}
	}

	return(true);
}

bool fillArchiveNameVector(vector<string>& pArchiveNames)
{
	if(!hasInputPath)
		getGamePath();

	vector<string> locales, languageLocales;

	languageLocales.push_back("enGB");
	languageLocales.push_back("enUS");
	languageLocales.push_back("deDE");
	languageLocales.push_back("esES");
	languageLocales.push_back("frFR");
	languageLocales.push_back("ruRU");

	for(vector<string>::iterator i = languageLocales.begin(); i != languageLocales.end(); ++i)
	{
		string localePath = gamepath + *i;
		struct stat status;
		if(stat(localePath.c_str(), &status))
			continue;
		if((status.st_mode & S_IFDIR) == 0)
			continue;
		gLog("[World of Warcraft Studio - Editor] - Found locales '%s'\n", i->c_str());
		locales.push_back(*i);
	}

	gLog("[World of Warcraft Studio - Editor] - Addind data files from locale dir.\n");
	for(vector<string>::iterator i = locales.begin(); i != locales.end(); ++i)
	{
		switch(loadExpansion())
		{
		case 1: // TBC
			{
				pArchiveNames.push_back(gamepath + string("common.MPQ"));
				pArchiveNames.push_back(gamepath + string("expansion.MPQ"));
				pArchiveNames.push_back(gamepath + string("patch.MPQ"));
				pArchiveNames.push_back(gamepath + string("patch-2.MPQ"));

				pArchiveNames.push_back(gamepath + *i + "\\locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\expansion-locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\patch-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\patch-" + *i + "-2.MPQ");

				break;
			}

		case 2: // WotLK
			{
				pArchiveNames.push_back(gamepath + string("common.MPQ"));
				pArchiveNames.push_back(gamepath + string("common-2.MPQ"));
				pArchiveNames.push_back(gamepath + string("expansion.MPQ"));
				pArchiveNames.push_back(gamepath + string("lichking.MPQ"));
				pArchiveNames.push_back(gamepath + string("patch.MPQ"));
				pArchiveNames.push_back(gamepath + string("patch-2.MPQ"));

				pArchiveNames.push_back(gamepath + *i + "\\locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\expansion-locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\lichking-locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\patch-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\patch-" + *i + "-2.MPQ");

				break;
			}

		case 3: // Cataclysm
			{
				pArchiveNames.push_back(gamepath + string("art.MPQ"));
				pArchiveNames.push_back(gamepath + string("expansion1.MPQ"));
				pArchiveNames.push_back(gamepath + string("expansion2.MPQ"));
				pArchiveNames.push_back(gamepath + string("expansion3.MPQ"));
				pArchiveNames.push_back(gamepath + string("sound.MPQ"));
				pArchiveNames.push_back(gamepath + string("world.MPQ"));

				pArchiveNames.push_back(gamepath + *i + "\\locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\expansion1-locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\expansion2-locale-" + *i + ".MPQ");
				pArchiveNames.push_back(gamepath + *i + "\\expansion3-locale-" + *i + ".MPQ");

				break;
			}

		default: // ERROR
			{
				gLog("[World of Warcraft Studio - Editor] - Can't load GameVersion.\n");
				exit(1);

				break;
			}
		}
	}

	gLog("[World of Warcraft Studio - Editor] - Scanning patch levels from data dir.\n");
	sprintf(path, "%spatch", gamepath);
	if(!scan_patches(path, pArchiveNames))
		return(false);

	gLog("[World of Warcraft Studio - Editor] - Scanning patch levels from locale dir.\n");
	bool foundOne = false;
	for(vector<string>::iterator i = locales.begin(); i != locales.end(); ++i)
	{
		sprintf(path, "%s%s/patch-%s", InputPath, i->c_str(), i->c_str());
		if(scan_patches(path, pArchiveNames))
			foundOne = true;
	}

	if(!foundOne)
	{
		gLog("[World of Warcraft Studio - Editor] - Locales not found\n");
		return false;
	}

	return true;
}

void CreateStrips();
void InitGroundEffects();
void InitAreaDB();
int main(int argc, char *argv[])
{
	srand((unsigned int)time(0));

	int xres = 1024;
	int yres = 768;

	bool usePatch = true;

	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i],"-f")) 
			fullscreen = 1;
		
		else if (!strcmp(argv[i],"-w")) 
			fullscreen = 0;
		
		else if (!strcmp(argv[i],"-1024") || !strcmp(argv[i],"-1024x768"))
		{
			xres = 1024;
			yres = 768;
		}
		
		else if (!strcmp(argv[i],"-1280") || !strcmp(argv[i],"-1280x1024"))
		{
			xres = 1280;
			yres = 1024;
		}
		
		else if (!strcmp(argv[i],"-1280x960"))
		{
			xres = 1280;
			yres = 960;
		}
		
		else if (!strcmp(argv[i],"-1400") || !strcmp(argv[i],"-1400x1050"))
		{
			xres = 1400;
			yres = 1050;
		}
		
		else if (!strcmp(argv[i],"-1280x800"))
		{
			xres = 1280;
			yres = 800;
		}
		
		else if (!strcmp(argv[i],"-1600") || !strcmp(argv[i],"-1600x1200"))
		{
			xres = 1600;
			yres = 1200;
		}
		
		else if (!strcmp(argv[i],"-1920") || !strcmp(argv[i],"-1920x1200"))
		{
			xres = 1920;
			yres = 1200;
		}
		
		else if (!strcmp(argv[i],"-2048") || !strcmp(argv[i],"-2048x1536"))
		{
			xres = 2048;
			yres = 1536;
		}
		
		else if (!strcmp(argv[i],"-p"))
			usePatch = true;
		
		else if (!strcmp(argv[i],"-np"))
			usePatch = false;
	}

	checkConfig();

	/*if(!loadPath())
	{
		getGamePath();
	}
	else
	{
		getGamePath() = loadPath();
	}*/

	getGamePath();
	CreateStrips();

	gLog("[World of Warcraft Studio - Editor] - " APP_TITLE " : " APP_VERSION "\n[World of Warcraft Studio - Editor] - Game path: %s\n[World of Warcraft Studio - Editor] - Game Version: %d\n", gamepath, loadGameVersion());
	GraphicCard(); // Send to Log info about Graphic Card

	checkConfig2();

	vector<string> archiveNames;
	fillArchiveNameVector(archiveNames);
	for(size_t i = 0; i < archiveNames.size(); ++i)
	{
		MPQArchive *archive = new MPQArchive(archiveNames[i].c_str());
	}

	if(gOpenArchives.empty())
	{
		gLog("[World of Warcraft Studio - Editor] - ERROR: No one archive found");
		return 1;
	}

	gAreaDB.open();

	video.init(xres, yres, fullscreen != 0);

	gLog("[World of Warcraft Studio - Editor] - Initializing Ground Effects\n");
	InitGroundEffects();
	gLog("[World of Warcraft Studio - Editor] - Initializing Fonts\n");
	InitFonts();
	gLog("[World of Warcraft Studio - Editor] - Main Initializing complete\n");

	float ftime;
	uint32 t, last_t, frames = 0, time = 0, fcount = 0, ft = 0;
	AppState* as;
	gFPS = 0;

	gLog("[World of Warcraft Studio - Editor] - Creating Menu\n");
	Menu* m = new Menu();
	as = m;

	gStates.push_back(as);

	if(glExtGetGLProcs_VertexProgram_1_0_ARB() == 0)
	{
		gLog("[World of Warcraft Studio - Editor] - Unable to load ARB Vertex Program Code\n");
		return 0;
	}
	loadWaterShader();

	bool done = false;
	t = SDL_GetTicks();
	gLog("[World of Warcraft Studio - Editor] - Entering Main Loop\n");
	while(gStates.size() > 0 && !done)
	{
		last_t = t;
		t = SDL_GetTicks();
		Uint32 dt = t - last_t;
		time += dt;
		ftime = time / 1000.0f;

		as = gStates[gStates.size()-1];

		SDL_Event event;
		while ( SDL_PollEvent(&event) )
		{
			if ( event.type == SDL_QUIT )
				done = true;

			else if ( event.type == SDL_MOUSEMOTION)
			{
				if(SDL_GetAppState()&SDL_APPMOUSEFOCUS)
					as->mousemove(&event.motion);
			}
			
			else if ( (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)&&(SDL_GetAppState()&SDL_APPINPUTFOCUS))
			{	
				if(event.button.type == SDL_MOUSEBUTTONUP)
					as->mouseclick(&event.button);
				else if(SDL_GetAppState()&SDL_APPMOUSEFOCUS)
					as->mouseclick(&event.button);
			}
			
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			{
				if(SDL_GetAppState()&SDL_APPINPUTFOCUS)
					as->keypressed(&event.key);
			}
			
			else if (event.type == SDL_VIDEORESIZE)
				video.resize(event.resize.w,event.resize.h);
		}

		if(SDL_GetAppState()&SDL_APPACTIVE)
		{
			as->tick(ftime, dt / 1000.0f);
			as->display(ftime, dt / 1000.0f);
		}

		if (gPop) 
		{
			gPop = false;
			gStates.pop_back();
			delete as;
		}

		frames++;
		fcount++;
		ft += dt;
		
		if (ft >= 1000)
		{
            float fps = (float)fcount / (float)ft * 1000.0f;
			gFPS = fps;
			char buf[32];
			sprintf_s(buf, APP_TITLE " - %.2f fps", fps);
			SDL_WM_SetCaption(buf, NULL);
            ft = 0;
			fcount = 0;
		}

		video.flip();

	}
	gLog("[World of Warcraft Studio - Editor] - Exited Main Loop\n");

	deleteFonts();
	
	video.close();

	for (deque<MPQArchive*>::iterator it = archives.begin(); it != archives.end(); ++it)
        (*it)->close();
		
	archives.clear();

	gLog("\n[World of Warcraft Studio - Editor] - Exiting.\n");

	return 0;
}

float frand()
{
    return rand() / (float)RAND_MAX;
}

float randfloat (float lower, float upper)
{
	return lower + (upper-lower)*(rand()/(float)RAND_MAX);
}

int randint (int lower, int upper)
{
    return lower + (int)((upper+1-lower)*frand());
}

void fixnamen(char *name, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		if (i > 0 && name[i] >= 'A' && name[i] <= 'Z' && isalpha(name[i-1]))
			name[i] |= 0x20;

		else if ((i == 0 || !isalpha(name[i-1])) && name[i] >= 'a' && name[i] <= 'z')
			name[i] &= ~0x20;
	}
}

void fixname(string &name)
{
	for (size_t i = 0; i < name.length(); ++i)
	{
		if (i > 0 && name[i] >= 'A' && name[i] <= 'Z' && isalpha(name[i-1]))
			name[i] |= 0x20;

		else if ((i == 0 || !isalpha(name[i-1])) && name[i] >= 'a' && name[i] <= 'z')
			name[i] &= ~0x20;
	}
}