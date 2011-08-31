#ifdef _WIN32
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")

#define NOMINMAX
#include <windows.h>
#include <winerror.h>
#endif

#ifdef _WIN64
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")

#define NOMINMAX
#include <windows.h>
#include <winerror.h>
#endif

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "Config.h"

#include "MPQ.h"
#include "Video.h"
#include "Appstate.h"

#include "Test.h"
#include "Menu.h"
#include "Areadb.h"

#include "Shaders.h"

int fullscreen = 0;

// ##################
// ## CHECK CONFIG ##
// ##################
#define CONFIG_FILE "Editor.conf"
int checkConfig()
{
	FILE *EditorPTR;

	EditorPTR = fopen(CONFIG_FILE, "r");

	if(EditorPTR) // If open Editor
	{
		gLog("[World of Warcraft Studio - Editor] - Config exist\n");

		if(EditorPTR == NULL) // If conf is empty
		{
			gLog("[World of Warcraft Studio - Editor] - Config is empty\n");
			exit(1);
		}
		else
		{
			gLog("[World of Warcraft Studio - Editor] - Config have content\n"); // Config isn't empty
		}
	}
	else
	{
		gLog("[World of Warcraft Studio - Editor] - Config doesn't exist\n"); // Config doesn't exist
		exit(1);
	}

	return 0;
}

// ######################
// ## EXPANSION SELECT ##
// ######################
int loadExpansion()
{
	Config conf(CONFIG_FILE);

	int expansion;

	expansion = conf.Value("expansion_option", "Expansion");

	return 0;
}

// ######################
// ## GAMEPATH  SELECT ##
// ######################
bool loadPath()
{
	Config conf(CONFIG_FILE);

	char configPath;

	configPath = conf.Value("game_option", "Path");

	return 0;
}

// ######################
// ## GAMEVER - SELECT ##
// ######################
int loadGameVersion()
{
	Config conf(CONFIG_FILE);

	char gameVersion;

	gameVersion = conf.Value("game_option", "GameVersion");

	return 0;
}

// ######################
// ## LANGUAGE  SELECT ##
// ######################
int loadLanguage()
{
	Config conf(CONFIG_FILE);

	int Language;

	Language = conf.Value("language_option", "Language");

	return 0;
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
		if(loadExpansion() != 1 || 2 || 3)
		{
			gLog("[World of Warcraft Studio - Editor] - Expansion isn't selected\n"); 
			exit(1);
		}
		else
		{
			gLog("[World of Warcraft Studio - Editor] - Expansion is selected - %s\n", loadExpansion());
		}

		if(!loadPath())
		{
			gLog("[World of Warcraft Studio - Editor] - Path is selected from Registry\n");
		}
		else
		{
			gLog("[World of Warcraft Studio - Editor] - Path is selected from Config - %s\n", loadPath());
		}

		if(loadLanguage() != 1 || 2 || 3)
		{
			gLog("[World of Warcraft Studio - Editor] - Language isn't selected\n");
			exit(1);
		}
		else
		{
			gLog("[World of Warcraft Studio - Editor] - Language is selected - %s\n", loadLanguage());
		}
	}

	return 0;
}

std::vector<AppState*> gStates;
bool gPop = false;

char gamepath[1024];
char wowpath[1024];


float gFPS;

GLuint ftex;
Font *f16, *f24, *f32;
freetype::font_data arialn13,arial12,arial14,arial16,morpheus40;	
AreaDB gAreaDB;


void initFonts()
{
	ftex = loadTGA("arial.tga",false);


	f16 = new Font(ftex, 256, 256, 16, "arial.info");
	f24 = new Font(ftex, 256, 256, 24, "arial.info");
	f32 = new Font(ftex, 256, 256, 32, "arial.info");

	
	morpheus40.initMPQ("fonts\\MORPHEUS.TTF",40);
	arialn13.initMPQ("fonts\\arialn.TTF",13);
	
	arial12.init("arial.ttf",12);
	arial14.init("arial.ttf",14);
	arial16.init("arial.ttf",16);
	
}

void deleteFonts()
{
	glDeleteTextures(1, &ftex);

	delete f16;
	delete f24;
	delete f32;
}



FILE *flog;
bool glogfirst = true;


void gLog(char *str, ...)
{
	if (glogfirst) {
		flog = fopen("log.txt","w");
		fclose(flog);
		glogfirst = false;
	}

	flog = fopen("log.txt","a");

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

#ifdef _WIN64
	HKEY key;
	DWORD t,s;
	LONG l;
	s = 1024;
	memset(gamepath,0,s);
	l = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Wow6432Node\\Blizzard Entertainment\\World of Warcraft",0,KEY_QUERY_VALUE,&key);
	l = RegQueryValueEx(key,"InstallPath",0,&t,(LPBYTE)gamepath,&s);
	l = RegQueryValueEx(key,"InstallPath",0,&t,(LPBYTE)wowpath,&s);
	RegCloseKey(key);
	strcat_s(gamepath,"Data\\");
#else
	strcpy(gamepath,"data/");
#endif
}

void CreateStrips();
void InitGroundEffects();
int main(int argc, char *argv[])
{
	srand((unsigned int)time(0));

	int xres = 1024;
	int yres = 768;
	//int xres = 1152;
	//int yres = 864;

	bool usePatch = true;

	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i],"-f")) fullscreen = 1;
		else if (!strcmp(argv[i],"-w")) fullscreen = 0;
		else if (!strcmp(argv[i],"-1024") || !strcmp(argv[i],"-1024x768")) {
			xres = 1024;
			yres = 768;
		}
		else if (!strcmp(argv[i],"-1280") || !strcmp(argv[i],"-1280x1024")) {
			xres = 1280;
			yres = 1024;
		}
		else if (!strcmp(argv[i],"-1280x960")) {
			xres = 1280;
			yres = 960;
		}
		else if (!strcmp(argv[i],"-1400") || !strcmp(argv[i],"-1400x1050")) {
			xres = 1400;
			yres = 1050;
		}
		else if (!strcmp(argv[i],"-1280x800")) {
			xres = 1280;
			yres = 800;
		}
		else if (!strcmp(argv[i],"-1600") || !strcmp(argv[i],"-1600x1200")) {
			xres = 1600;
			yres = 1200;
		}
		else if (!strcmp(argv[i],"-1920") || !strcmp(argv[i],"-1920x1200")) {
			xres = 1920;
			yres = 1200;
		}
		else if (!strcmp(argv[i],"-2048") || !strcmp(argv[i],"-2048x1536")) {
			xres = 2048;
			yres = 1536;
		}
		else if (!strcmp(argv[i],"-p")) usePatch = true;
		else if (!strcmp(argv[i],"-np")) usePatch = false;
	}

	gLog("[World of Warcraft Studio - Editor] - " APP_TITLE " - " APP_VERSION "\n[World of Warcraft Studio - Editor] - Game path: %s\n", gamepath);
	gLog("[World of Warcraft Studio - Editor] - %s\n[World of Warcraft Studio - Editor] - %s\n[World of Warcraft Studio - Editor] - %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	
	checkConfig();

	/*if(!loadPath())
	{
		getGamePath();
	}*/
	CreateStrips();

	loadExpansion();
	checkConfig2();

	std::vector<MPQArchive*> archives;
	bool archiveNames[] = {""};
	if(loadExpansion() == 1) // TBC
	{
		bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", "patch-3.MPQ"};
	}
	else if(loadExpansion() == 2) // WotLK
	{
		if(loadGameVersion() == 1) // enGB
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"enGB/locale-enGB.MPQ", "enGB/expansion-locale-enGB.MPQ", "enGB/lichking-locale-enGB.MPQ", "enGB/patch-enGB.MPQ", "enGB/patch-enGB-2.MPQ"};
		}
		else if(loadGameVersion() == 2) // enUS
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"enUS/locale-enUS.MPQ", "enUS/expansion-locale-enUS.MPQ", "enUS/lichking-locale-enUS.MPQ", "enUS/patch-enUS.MPQ", "enUS/patch-enUS-2.MPQ"};
		}
		else if(loadGameVersion() == 3) // deDE
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"deDE/locale-deDE.MPQ", "deDE/expansion-locale-deDE.MPQ", "deDE/lichking-locale-deDE.MPQ", "deDE/patch-deDE.MPQ", "deDE/patch-deDE-2.MPQ"};
		}
		else if(loadGameVersion() == 4) // esES
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"esES/locale-esES.MPQ", "esES/expansion-locale-esES.MPQ", "esES/lichking-locale-esES.MPQ", "esES/patch-esES.MPQ", "esES/patch-esES-2.MPQ"};
		}
		else if(loadGameVersion() == 5) // frFR
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"frFR/locale-frFR.MPQ", "frFR/expansion-locale-frFR.MPQ", "frFR/lichking-locale-frFR.MPQ", "frFR/patch-frFR.MPQ", "frFR/patch-frFR-2.MPQ"};
		}
		else if(loadGameVersion() == 6) // ruRU
		{
			bool archiveNames[] = {"common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ", "patch.MPQ", "patch-2.MPQ", 
			"ruRU/locale-ruRU.MPQ", "ruRU/expansion-locale-ruRU.MPQ", "ruRU/lichking-locale-ruRU.MPQ", "ruRU/patch-ruRU.MPQ", "ruRU/patch-ruRU-2.MPQ"};
		}
		else
		{
			gLog("[World of Warcraft Studio - Editor] - Can't load GameVersion.");
			exit(1);
		}

		if(loadGameVersion())
		{
			gLog("[World of Warcraft Studio - Editor] - GameVersion is selected - %s\n", loadGameVersion());
		}
	}
	else if(loadExpansion() == 3) // Cataclysm
	{
		bool archiveNames[] = {"art.MPQ", "expansion1.MPQ", "expansion2.MPQ", "expansion3.MPQ", "sound.MPQ", "world.MPQ"};
	}
	else
	{
		gLog("Expansion isn't - The Burning Crusade or Wrath of the Lich King or Cataclysm. Select one of third expansion.");
		exit(1);
	}

	char path[512];

	//Patches
	/*
	if (usePatch) {
		// patch goes first -> fake priority handling
		sprintf(path, "%s%s", gamepath, "patch-9.MPQ");
		archives.push_back(new MPQArchive(path));
		sprintf(path, "%s%s", gamepath, "patch-8.MPQ");
		archives.push_back(new MPQArchive(path));
		sprintf(path, "%s%s", gamepath, "patch-7.MPQ");
		archives.push_back(new MPQArchive(path));
		sprintf(path, "%s%s", gamepath, "patch-6.MPQ");
		archives.push_back(new MPQArchive(path));
		sprintf(path, "%s%s", gamepath, "patch-5.MPQ");
		archives.push_back(new MPQArchive(path));
		sprintf(path, "%s%s", gamepath, "patch-4.MPQ");
		archives.push_back(new MPQArchive(path));
	}*/

	for (size_t i=0; i<7; i++) {
		sprintf_s(path, "%s%s", gamepath, archiveNames[i]);
		archives.push_back(new MPQArchive(path));
	}

	gAreaDB.open();

	video.init(xres,yres,fullscreen!=0);
	SDL_WM_SetCaption(APP_TITLE,NULL);


	gLog("[World of Warcraft Studio - Editor] - Initializing Ground Effects\n");
	InitGroundEffects();
	gLog("[World of Warcraft Studio - Editor] - Initializing Fonts\n");
	initFonts();

	float ftime;
	Uint32 t, last_t, frames = 0, time = 0, fcount = 0, ft = 0;
	AppState *as;
	gFPS = 0;

	gLog("[World of Warcraft Studio - Editor] - Creating Menu\n");
	Menu *m = new Menu();
	as = m;

	gStates.push_back(as);

	if(glExtGetGLProcs_VertexProgram_1_0_ARB()==0)
	{
		gLog("[World of Warcraft Studio - Editor] - Unable to load ARB Vertex Program Code\n");
		return 0;
	}
	loadWaterShader();

	bool done = false;
	t = SDL_GetTicks();
	gLog("[World of Warcraft Studio - Editor] - Entering Main Loop\n");
	while(gStates.size()>0 && !done) {
		last_t = t;
		t = SDL_GetTicks();
		Uint32 dt = t - last_t;
		time += dt;
		ftime = time / 1000.0f;

		as = gStates[gStates.size()-1];

		SDL_Event event;
		while ( SDL_PollEvent(&event) ) {
			if ( event.type == SDL_QUIT ) {
				done = true;
			}
			else if ( event.type == SDL_MOUSEMOTION) {
				if(SDL_GetAppState()&SDL_APPMOUSEFOCUS)
					as->mousemove(&event.motion);
			}
			else if ( (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)&&(SDL_GetAppState()&SDL_APPINPUTFOCUS)) {
				
				if(event.button.type == SDL_MOUSEBUTTONUP)
					as->mouseclick(&event.button);
				else if(SDL_GetAppState()&SDL_APPMOUSEFOCUS)
					as->mouseclick(&event.button);
			}
			else if ( (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)) {
				if(SDL_GetAppState()&SDL_APPINPUTFOCUS)
					as->keypressed(&event.key);
			}
			else if( event.type == SDL_VIDEORESIZE)
			{
				video.resize(event.resize.w,event.resize.h);
			}
		}



		if(SDL_GetAppState()&SDL_APPACTIVE)
		{
			as->tick(ftime, dt/1000.0f);

			as->display(ftime, dt/1000.0f);
		}

		if (gPop) {
			gPop = false;
			gStates.pop_back();
			delete as;
		}

		frames++;
		fcount++;
		ft += dt;
		if (ft >= 1000) {
            float fps = (float)fcount / (float)ft * 1000.0f;
			gFPS = fps;
			char buf[32];
			sprintf_s(buf, APP_TITLE " - %.2f fps",fps);
			SDL_WM_SetCaption(buf,NULL);
            ft = 0;
			fcount = 0;
		}

		video.flip();

	}
	gLog("[World of Warcraft Studio - Editor] - Exited Main Loop\n");

	deleteFonts();
	
	video.close();

	for (std::vector<MPQArchive*>::iterator it = archives.begin(); it != archives.end(); ++it) {
        (*it)->close();
	}
	archives.clear();

	gLog("\n[World of Warcraft Studio - Editor] - Exiting.\n");

	return 0;
}

float frand()
{
    return rand()/(float)RAND_MAX;
}

float randfloat(float lower, float upper)
{
	return lower + (upper-lower)*(rand()/(float)RAND_MAX);
}

int randint(int lower, int upper)
{
    return lower + (int)((upper+1-lower)*frand());
}

void fixnamen(char *name, size_t len)
{
	for (size_t i=0; i<len; i++) {
		if (i>0 && name[i]>='A' && name[i]<='Z' && isalpha(name[i-1])) {
			name[i] |= 0x20;
		} else if ((i==0 || !isalpha(name[i-1])) && name[i]>='a' && name[i]<='z') {
			name[i] &= ~0x20;
		}
	}
}

void fixname(std::string &name)
{
	for (size_t i=0; i<name.length(); i++) {
		if (i>0 && name[i]>='A' && name[i]<='Z' && isalpha(name[i-1])) {
			name[i] |= 0x20;
		} else if ((i==0 || !isalpha(name[i-1])) && name[i]>='a' && name[i]<='z') {
			name[i] &= ~0x20;
		}
	}
}