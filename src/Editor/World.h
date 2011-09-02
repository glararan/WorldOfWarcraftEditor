#ifndef WORLD_H
#define WORLD_H

#include "OpenGL.h"
#include "MapTile.h"
#include "WMO.h"
#include "Frustrum.h"
#include "Sky.h"
#include "Selection.h"

#include <string>

#define MAPTILECACHESIZE 36

const float detail_size = 8.0f;

class World {

	MapTile *maptilecache[MAPTILECACHESIZE];
	MapTile *current[5][5];
	int ex,ez;

	

public:
	float LowerTerrain;
	float LastRaise;
	float zoom;
	bool SelectionMode;
	int hits;
	
	GLuint Dist;
	
	std::string basename;

	bool maps[64][64];
	GLuint lowrestiles[64][64];
	bool autoheight;

	std::vector<std::string> gwmos;
	std::vector<WMOInstance> gwmois;
	int gnWMO, nMaps;

	float mapdrawdistance, modeldrawdistance, doodaddrawdistance, highresdistance;
	float mapdrawdistance2, modeldrawdistance2, doodaddrawdistance2, highresdistance2;

	float culldistance, culldistance2, fogdistance;

	float l_const, l_linear, l_quadratic;

	Skies *skies;
	float time,animtime;

    bool hadSky;

	bool thirdperson,lighting,drawmodels,drawdoodads,drawterrain,drawwmo,loading,drawhighres,drawfog,drawlines;
	bool uselowlod;

	GLuint detailtexcoords, alphatexcoords;

	short *mapstrip,*mapstrip2;

	TextureID water;
	Vec3D camera, lookat;
	Frustum frustum;
	int cx,cz;
	bool oob;

	WMOManager wmomanager;
	ModelManager modelmanager;

	OutdoorLighting *ol;
	OutdoorLightStats outdoorLightStats;

	GLuint minimap;

	World(const char* name);
	~World();
	void init();
	void initMinimap();
	void initDisplay();
	void initWMOs();
	void initLowresTerrain();

	void onTheFlyLoading();

	void enterTile(int x, int z);
	void enterTileInit(int x, int z);
	bool tileLoaded(int x, int z);
	MapTile *loadTile(int x, int z);
	void reloadTile(int x, int z);
	void saveTile(int x, int z);
	void tick(float dt);
	void draw();
	
	void outdoorLighting();
	void outdoorLighting2();
	void outdoorLights(bool on);
	void setupFog();

	/// Get the tile on wich the camera currently is on
	unsigned int getAreaID();

	void drawSelection(int cursorX,int cursorY);
	void drawSelectionChunk(int cursorX,int cursorY);
	void drawTileMode(float ah);
	
	int getSelection();

	GLuint	CurTex;
	GLuint getSelectTex(){return CurTex;};
	bool GetVertex(float x,float z, Vec3D *V);
	void changeTerrain(float x, float z, float change, float radius, int BrushType);
	void flattenTerrain(float x, float z, float h, float remain, float radius, int BrushType);	
	void blurTerrain(float x, float z, float remain, float radius, int BrushType);
	void paintTexture(float x, float z, brush *Brush, float strength, float pressure, int texture);
	void eraseTextures(float x, float z);
	void saveMap();

	float minX,maxX,minY,maxY;
};

extern World *gWorld;

void lightingDefaults();
void myFakeLighting();

#endif