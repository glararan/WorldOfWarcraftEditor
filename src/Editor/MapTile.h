#ifndef MAPTILE_H
#define MAPTILE_H

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)
#define UNITSIZE (CHUNKSIZE / 8.0f)
#define ZEROPOINT (32.0f * (TILESIZE))

#include "Video.h"
#include "MPQ.h"
#include "WMO.h"
#include "Model.h"
#include "Liquid.h"
#include "Brush.h"
#include <vector>
#include <string>
#include "MapHeaders.h"

class MapTile;
class MapChunk;

class World;
#define FLAG_SHADOW		1
#define	FLAG_IMPASS		2
#define	FLAG_LQ_RIVER	4
#define	FLAG_LQ_OCEAN	8
#define	FLAG_LQ_MAGMA	16

const int mapbufsize = 9*9 + 8*8;

class MapNode {
public:

	MapNode(int x, int y, int s):px(x),py(y),size(s) {}

	int px, py, size;

	Vec3D vmin, vmax, vcenter;

	MapNode *children[4];
	MapTile *mt;

	virtual void draw();
	virtual void drawSelect();
	virtual void drawColor();
	void setup(MapTile *t);
	void cleanup();

};

struct MapChunkHeader
{
	uint32 flags;
	uint32 ix;
	uint32 iy;
	uint32 nLayers;
	uint32 nDoodadRefs;
	uint32 ofsHeight;
	uint32 ofsNormal;
	uint32 ofsLayer;
	uint32 ofsRefs;
	uint32 ofsAlpha;
	uint32 sizeAlpha;
	uint32 ofsShadow;
	uint32 sizeShadow;
	uint32 areaid;
	uint32 nMapObjRefs;
	uint32 holes;
	uint16 s1;
	uint16 s2;
	uint32 d1;
	uint32 d2;
	uint32 d3;
	uint32 predTex;
	uint32 nEffectDoodad;
	uint32 ofsSndEmitters;
	uint32 nSndEmitters;
	uint32 ofsLiquid;
	uint32 sizeLiquid;
	float  zpos;
	float  xpos;
	float  ypos;
	uint32 textureId;
	uint32 props;
	uint32 effectId;
};

class MapChunk : public MapNode
{
public:
	MapChunkHeader header;
	bool Changed;
	int nTextures;

	float xbase, ybase, zbase;
	float r;

	unsigned int nameID;

	unsigned int Flags;
	

	unsigned int areaID;

	bool haswater;
	bool visible;
	bool hasholes;
	float waterlevel[2];

	bool	deleted;

	int				tex[4];
	TextureID		textures[4];
	unsigned int	texFlags[4];
	unsigned int	effectID[4];
	unsigned char amap[3][64*64];
	TextureID alphamaps[3];
	TextureID shadow;

	int animated[4];

	GLuint vertices, normals, minimap, minishadows;

	short *strip;
	int striplen;

	Liquid *lq;

	MapChunk():MapNode(0,0,0){};

	void init(MapTile* mt, MPQFile &f);
	void destroy();
	void initStrip(int holes);

	void draw();
	void drawContour();
	void drawColor();
	void drawSelect();
	void drawSelect2();
	void drawNoDetail();
	void drawPass(int anim);
	void drawWater();
	void drawLines();

	void drawTextures();

	void recalcNorms();

	Vec3D tn[mapbufsize], tv[mapbufsize], tm[mapbufsize];
	Vec4D ts[mapbufsize];

	void getSelectionCoord(float *x,float *z);
	float getSelectionHeight();
	void changeTerrain(float x, float z, float change, float radius, int BrushType);
	void flattenTerrain(float x, float z, float h, float remain, float radius, int BrushType);
	void blurTerrain(float x, float z, float remain, float radius, int BrushType);

	bool paintTexture(float x, float z, brush *Brush, float strength, float pressure, int texture);
	int addTexture(GLuint texture);
	void eraseTextures();

	bool GetVertex(float x,float z, Vec3D *V);

	void loadTextures();
//	char getAlpha(float x,float y);


	//float getTerrainHeight(float x, float z);
};

class MapTile
{
	std::string fname;
	
	MPQFile	*theFile;

	bool	chunksLoaded;
	int		nextChunk;
	size_t mcnk_offsets[256], mcnk_sizes[256];
	void	loadChunk();
	void	finishChunkLoad();

	bool	texturesLoaded;
	char	*textureBuffer;
	char	*texturePos;
	size_t	textureSize;

	void loadTexture();
	void finishTextureLoad();

	bool	modelsLoaded;
	char	*modelBuffer;
	char	*modelPos;
	size_t	modelSize;
	int		curModelID;
	
	uint32	modelNum;
	MDDF	*modelInstances;
	
	void loadModel();
	void loadModelInstances(int id);

	bool	wmosLoaded;
	char	*wmoBuffer;
	char	*wmoPos;
	size_t	wmoSize;
	
	uint32	wmoNum;
	MODF	*wmoInstances;
	

	void loadWMO();
	void loadWMOInstances();
	
public:
	void finishLoading();
	bool isLoaded(){return texturesLoaded&modelsLoaded&wmosLoaded;};
	void partialLoad(){
		if(!texturesLoaded)
		{
			for(int i=0;i<2;i++)
				loadTexture();
		}		
		else if(!chunksLoaded)
		{
			//loadChunk();
			loadChunk();
		}
		else if(!wmosLoaded)
			loadWMO();
		else if(!modelsLoaded)
			loadModel();		
	};
	std::vector<std::string> textures;
	std::vector<std::string> wmos;
	std::vector<std::string> models;

	std::vector<WMOInstance> wmois;
	std::vector<ModelInstance> modelis;
	int nWMO;
	int nMDX;

	int x, z;
	bool ok;

	//World *world;

	float xbase, zbase;

	MapChunk chunks[16][16];

	MapNode topnode;

	MapTile(int x0, int z0, char* filename);
	~MapTile();

	void draw();
	void drawSelect();
	void drawLines();
	void drawWater();
	void drawObjects();
	void drawColored();
	void drawObjectsSelect();
	void drawSky();
	//void drawPortals();
	void drawModels();
	void drawModelsMapTile();
	void drawModelsSelect();
	void drawTextures();

	bool GetVertex(float x,float z, Vec3D *V);
	

	void saveTile();

	/// Get chunk for sub offset x,z
	MapChunk *getChunk(unsigned int x, unsigned int z);
};

int indexMapBuf(int x, int y);

// 8x8x2 version with triangle strips, size = 8*18 + 7*2
const int stripsize = 8*18 + 7*2;
template <class V>
void stripify(V *in, V *out)
{
	for (int row=0; row<8; row++) {
		V *thisrow = &in[indexMapBuf(0,row*2)];
		V *nextrow = &in[indexMapBuf(0,(row+1)*2)];

		if (row>0) *out++ = thisrow[0];
		for (int col=0; col<9; col++) {
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		if (row<7) *out++ = nextrow[8];
	}
}

// high res version, size = 16*18 + 7*2 + 8*2
const int stripsize2 = 16*18 + 7*2 + 8*2;
template <class V>

void stripify2(V *in, V *out)
{
	for (int row=0; row<8; row++) { 
		V *thisrow = &in[indexMapBuf(0,row*2)];
		V *nextrow = &in[indexMapBuf(0,row*2+1)];
		V *overrow = &in[indexMapBuf(0,(row+1)*2)];

		if (row>0) *out++ = thisrow[0];// jump end
		for (int col=0; col<8; col++) {
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		*out++ = thisrow[8];
		*out++ = overrow[8];
		*out++ = overrow[8];// jump start
		*out++ = thisrow[0];// jump end
		*out++ = thisrow[0];
		for (int col=0; col<8; col++) {
			*out++ = overrow[col];
			*out++ = nextrow[col];
		}
		if (row<8) *out++ = overrow[8];
		if (row<7) *out++ = overrow[8];// jump start
	}
}

#endif