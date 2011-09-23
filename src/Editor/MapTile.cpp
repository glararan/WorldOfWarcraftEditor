#include "MapTile.h"
#include "World.h"
#include "Vec3D.h"
#include <cassert>
#include <algorithm>
#include "GroundEffects.h"

#define HEIGHT_TOP		1000
#define HEIGHT_MID		600
#define HEIGHT_LOW		300
#define HEIGHT_ZERO		0
#define HEIGHT_SHALLOW	-100
#define HEIGHT_DEEP		-250
#define MAPCHUNK_RADIUS	47.140452079103168293389624140323

bool DrawMapContour = false;
bool drawFlags = false;

/*
White	1.00	1.00	1.00
Brown	0.75	0.50	0.00
Green	0.00	1.00	0.00
Yellow	1.00	1.00	0.00
Lt Blue	0.00	1.00	1.00
Blue	0.00	0.00	1.00
Black	0.00	0.00	0.00
*/

void HeightColor(float height, Vec3D* Color)
{
	float Amount;
	
	if(height>HEIGHT_TOP)
	{
		Color->x = 1.0;
		Color->y = 1.0;
		Color->z = 1.0;
	}
	else if(height > HEIGHT_MID)
	{
		Amount=(height - HEIGHT_MID)/(HEIGHT_TOP - HEIGHT_MID);
		Color->x = .75f + Amount * 0.25f;
		Color->y = 0.5f + 0.5f * Amount;
		Color->z = Amount;
	}
	else if(height>HEIGHT_LOW)
	{
		Amount=(height - HEIGHT_LOW)/(HEIGHT_MID - HEIGHT_LOW);
		Color->x = Amount * 0.75f;
		Color->y = 1.00f - 0.5f * Amount;
		Color->z = 0.0f;
	}
	else if(height > HEIGHT_ZERO)
	{
		Amount = (height - HEIGHT_ZERO)/(HEIGHT_LOW - HEIGHT_ZERO);

		Color->x = 1.0f - Amount;
		Color->y = 1.0f;
		Color->z = 0.0f;
	}
	else if(height > HEIGHT_SHALLOW)
	{
		Amount = (height - HEIGHT_SHALLOW)/(HEIGHT_ZERO - HEIGHT_SHALLOW);
		Color->x = 0.0f;
		Color->y = Amount;
		Color->z = 1.0f;
	}
	else if(height > HEIGHT_DEEP)
	{
		Amount = (height - HEIGHT_DEEP)/(HEIGHT_SHALLOW - HEIGHT_DEEP);
		Color->x = 0.0f;
		Color->y = 0.0f;
		Color->z = Amount;
	}
	else
		(*Color) *= 0.0f;
}

TextureID Contour = 0;
float CoordGen[4];
#define CONTOUR_WIDTH 128

void GenerateContourMap()
{
	unsigned char CTexture[CONTOUR_WIDTH * 4];
	
	CoordGen[0] = 0.0f;
	CoordGen[1] = 0.25f;
	CoordGen[2] = 0.0f;
	CoordGen[3] = 0.0f;

	for(int i = 0; i < (CONTOUR_WIDTH * 4); i++)
		CTexture[i] = 0;

	CTexture[3 + CONTOUR_WIDTH/2] = 0xff;
	CTexture[7 + CONTOUR_WIDTH/2] = 0xff;
	CTexture[11 + CONTOUR_WIDTH/2] = 0xff;

	glGenTextures(1, &Contour);
	glBindTexture(GL_TEXTURE_2D, Contour);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, CONTOUR_WIDTH, 1, GL_RGBA, GL_UNSIGNED_BYTE, CTexture);
	/*glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,CONTOUR_WIDTH,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);
	
	for(int i=0;i<(CONTOUR_WIDTH*4)/2;i++)
		CTexture[i]=0;
	CTexture[3]=0xff;

	glTexImage1D(GL_TEXTURE_1D,1,GL_RGBA,CONTOUR_WIDTH/2,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);

	for(int i=0;i<(CONTOUR_WIDTH*4)/4;i++)
		CTexture[i]=0;
	CTexture[3]=0x80;

	glTexImage1D(GL_TEXTURE_1D,2,GL_RGBA,CONTOUR_WIDTH/4,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);

	for(int i=0;i<(CONTOUR_WIDTH*4)/8;i++)
		CTexture[i]=0;
	CTexture[3]=0x40;

	glTexImage1D(GL_TEXTURE_1D,3,GL_RGBA,CONTOUR_WIDTH/8,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);

	for(int i=0;i<(CONTOUR_WIDTH*4)/16;i++)
		CTexture[i]=0;
	CTexture[3]=0x20;

	glTexImage1D(GL_TEXTURE_1D,4,GL_RGBA,CONTOUR_WIDTH/16,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);

	for(int i=0;i<(CONTOUR_WIDTH*4)/32;i++)
		CTexture[i]=0;
	CTexture[3]=0x10;

	glTexImage1D(GL_TEXTURE_1D,5,GL_RGBA,CONTOUR_WIDTH/32,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);

	for(int i=0;i<(CONTOUR_WIDTH*4)/64;i++)
		CTexture[i]=0;
	CTexture[3]=0x08;

	glTexImage1D(GL_TEXTURE_1D,6,GL_RGBA,CONTOUR_WIDTH/64,0,GL_RGBA,GL_UNSIGNED_BYTE,CTexture);*/
	
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, CoordGen);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void startTimer();
void stopTimer();

MapTile::MapTile(int x0, int z0, char* filename) : x(x0), z(z0), topnode(0, 0, 16)
{
	xbase = x0 * TILESIZE;
	zbase = z0 * TILESIZE;

	startTimer();
	fname = filename;

	theFile = new MPQFile(filename);
	ok = !theFile->isEof();
	
	if(!ok)
	{
		gLog("[World of Warcraft Studio - Editor] - Loading tile %d, %d\n", x0, z0);
		gLog("[World of Warcraft Studio - Editor] - Error loading %s\n", filename);
		return;
	}

	if(!theFile->isExternal())
		gLog("[World of Warcraft Studio - Editor] - Loading tile from MPQ %d, %d\n", x0, z0);
	else
		gLog("[World of Warcraft Studio - Editor] - Loading tile from File %d, %d\n", x0, z0);

	char fourcc[5];
	size_t size;

	startTimer();
	while(!theFile->isEof())
	{
		theFile->read(fourcc, 4);
		theFile->read(&size, 4);

		flipcc(fourcc);
		fourcc[4] = 0;

		size_t nextpos = theFile->getPos() + size;

		if(!strcmp(fourcc, "MCIN"))
		{
			// mapchunk offsets/sizes
			for(int i = 0; i < 256; i++)
			{
				theFile->read(&mcnk_offsets[i], 4);
				theFile->read(&mcnk_sizes[i], 4);
				theFile->seekRelative(8);
			}
		}
		else if(!strcmp(fourcc, "MTEX"))
		{
			// texture lists
			if(size != 0)
			{
				textureSize = size;
				textureBuffer = new char[size];
				theFile->read(textureBuffer, size);
				texturePos = textureBuffer;
				texturesLoaded = false;
			}
			else
				texturesLoaded = true;
		}
		else if(!strcmp(fourcc, "MMDX"))
		{
			// models ...
			// MMID would be relative offsets for MMDX filenames
			if(size != 0)
			{
				modelSize = size;
				modelBuffer = new char[size];
				theFile->read(modelBuffer, size);
				modelPos = modelBuffer;
				modelsLoaded = false;
				curModelID = 0;
			}
			else
				modelsLoaded = true;

		}
		else if(!strcmp(fourcc, "MWMO"))
		{
			// map objects
			// MWID would be relative offsets for MWMO filenames			
			if(size != 0)
			{
				wmoSize = size;
				wmoBuffer = new char[size];
				theFile->read(wmoBuffer, size);
				wmoPos = wmoBuffer;
				wmosLoaded = false;
			}
			else
				wmosLoaded = true;
		}
		else if(!strcmp(fourcc, "MDDF"))
		{
			// model instance data
			modelNum = (int)size / 36;
			modelInstances = new MDDF[modelNum];
			theFile->read((unsigned char*)modelInstances, size);
		}
		else if(!strcmp(fourcc, "MODF"))
		{
			// wmo instance data
			wmoNum = (int)size / 64;
			wmoInstances = new MODF[wmoNum];
			theFile->read((unsigned char*)wmoInstances, size);
		}

		// MCNK data will be processed separately ^_^
		theFile->seek((int)nextpos);
	}
	gLog("[World of Warcraft Studio - Editor] - Finished Processing all but MCNK's in");
	stopTimer();

	//while(!texturesLoaded)
	//	loadTexture();

	// read individual map chunks
	/*for (int j=0; j<16; j++) {
		for (int i=0; i<16; i++) {
			f.seek((int)mcnk_offsets[j*16+i]);
			chunks[j][i].init(this, f);
		}
	}*/
	chunksLoaded = false;
	nextChunk = 0;
	
	gLog("[World of Warcraft Studio - Editor] - Entire loading of %s took ", fname.c_str());
	stopTimer();
}

void MapTile::loadChunk()
{	
	if(chunksLoaded)
		return;
	
	theFile->seek((int)mcnk_offsets[nextChunk]);
	chunks[nextChunk/16][nextChunk%16].init(this, *theFile);	
	gLog("[World of Warcraft Studio - Editor] - Loaded Chunk %d of %s\n", nextChunk, fname.c_str());
	nextChunk++;
	
	if(nextChunk == 256)
	{
		theFile->close();
		chunksLoaded = true;
		// init quadtree
		topnode.setup(this);
	}
}

void MapTile::loadTexture()
{
	if(texturesLoaded)
		return;
	
	if(textureSize <= 0)
	{
		texturesLoaded = true;
		return;
	}
	
	if(strlen(texturePos) > 0)
	{
		string texpath(texturePos);
		fixname(texpath);
		video.textures.add(texpath);
		textures.push_back(texpath);
	}

	texturePos += strlen(texturePos) + 1;

	if(texturePos >= textureBuffer + textureSize)
	{
		gLog("[World of Warcraft Studio - Editor] - Finished Loading Textures for %s\n", fname);
		texturesLoaded = true;
		//for(int i=0;i<256;i++)
		//	chunks[i/16][i%16].loadTextures();
		delete textureBuffer;				
	}
}

void MapTile::finishTextureLoad()
{
	while(!texturesLoaded)
		loadTexture();
}

void MapTile::loadModel()
{
	if(modelsLoaded)
		return;
	
	if(modelSize <= 0)
	{
		modelsLoaded = true;
		return;
	}
	
	if(strlen(modelPos) > 0)
	{
		string path(modelPos);
		fixname(path);
		gWorld->modelmanager.add(path);
		models.push_back(path);
		loadModelInstances(curModelID);
		curModelID++;
	}

	modelPos += strlen(modelPos) + 1;

	if(modelPos >= modelBuffer+modelSize)
	{
		gLog("[World of Warcraft Studio - Editor] - Finished Loading Models for %s\n", fname);
		modelsLoaded = true;
		//Need to load Model Instances now
		//loadModelInstances();
		delete modelInstances;
		delete modelBuffer;				
	}
}

void MapTile::loadModelInstances(int id)
{
	for (int i = 0; i < modelNum; i++)
	{
		if(modelInstances[i].nameID != id)
			continue;

		Model* model = (Model*)gWorld->modelmanager.items[gWorld->modelmanager.get(models[modelInstances[i].nameID])];
		ModelInstance inst(model, &modelInstances[i]);
		inst.modelID=modelInstances[i].nameID;
		//addModelToList(model,f);
		modelis.push_back(inst);
		nMDX++;
	}	
}

/*void MapTile::loadModelInstances()
{
	for (int i=0; i<modelNum; i++)
	{
		Model *model = (Model*)gWorld->modelmanager.items[gWorld->modelmanager.get(models[modelInstances[i].nameID])];
		ModelInstance inst(model, &modelInstances[i]);
		inst.modelID=modelInstances[i].nameID;
		//addModelToList(model,f);
		modelis.push_back(inst);
	}
	nMDX=modelNum;
	delete modelInstances;
}*/

void MapTile::loadWMO()
{
	if(wmosLoaded)
		return;	

	if(wmoSize <= 0)
	{
		wmosLoaded = true;
		return;
	}
	
	if(strlen(wmoPos) > 0)
	{
		string path(wmoPos);
		fixname(path);
		gWorld->wmomanager.add(path);
		wmos.push_back(path);
	}

	wmoPos += strlen(wmoPos) + 1;

	if(wmoPos >= wmoBuffer+wmoSize)
	{
		gLog("[World of Warcraft Studio - Editor] - Finished Loading WMOs for %s\n", fname);
		wmosLoaded = true;
		//Need to load WMO Instances now
		loadWMOInstances();
		delete wmoBuffer;				
	}
}

void MapTile::loadWMOInstances()
{
	for (int i = 0; i < wmoNum; i++)
	{
		WMO* wmo = (WMO*)gWorld->wmomanager.items[gWorld->wmomanager.get(wmos[wmoInstances[i].nameID])];
		WMOInstance inst(wmo, &wmoInstances[i]);
		wmois.push_back(inst);
	}
	nWMO = wmoNum;
	delete wmoInstances;
}

void MapTile::finishLoading()
{
	while(!texturesLoaded)
		loadTexture();
	while(!chunksLoaded)
		loadChunk();	
	while(!modelsLoaded)
		loadModel();
	while(!wmosLoaded)
		loadWMO();
}

MapTile::~MapTile()
{
	if(!ok)
		return;

	gLog("[World of Warcraft Studio - Editor] - Unloading tile %d,%d\n", x, z);

	if(chunksLoaded)
	{
		topnode.cleanup();

		for(int j = 0; j < 16; j++)
		{
			for(int i = 0; i < 16; i++)
			{
				chunks[j][i].destroy();
			}
		}
	}
	else
	{
		for(int i = 0; i < nextChunk; i++)
			chunks[i/16][i%16].destroy();
	}

	for(vector<string>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
        video.textures.delbyname(*it);
	}

	for(vector<string>::iterator it = wmos.begin(); it != wmos.end(); ++it)
	{
		gWorld->wmomanager.delbyname(*it);
	}

	for (vector<string>::iterator it = models.begin(); it != models.end(); ++it)
	{
		gWorld->modelmanager.delbyname(*it);
	}
}

void MapTile::draw()
{
	if(!texturesLoaded)
		finishTextureLoad();
	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}

	if(!ok)
		return;

	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			chunks[j][i].visible = false;
			chunks[j][i].draw();
		}
	}
	
	//topnode.draw();
}

void MapTile::drawSelect()
{	
	if(!ok)
		return;

	if(!texturesLoaded)
		finishTextureLoad();

	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}
	
	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			chunks[j][i].visible = false;
			chunks[j][i].drawSelect();
		}
	}
	//topnode.drawSelect();
}

void MapTile::drawColored()
{	
	if(!ok)
		return;

	if(!texturesLoaded)
		finishTextureLoad();

	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}
	
	/*for (int j=0; j<16; j++)
	{
		for (int i=0; i<16; i++)
		{
			chunks[j][i].visible = false;
			//chunks[j][i].draw();
		}
	}*/
	glDisable(GL_COLOR_MATERIAL);
	topnode.drawColor();
	glEnable(GL_COLOR_MATERIAL);
}


void MapTile::drawLines()
{
	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}

	glDisable(GL_COLOR_MATERIAL);
	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			//if(chunks[j][i].visible)
			chunks[j][i].drawLines();
		}
	}
	glEnable(GL_COLOR_MATERIAL);
}

void enableWaterShader();

void MapTile::drawWater()
{
//	if (!ok) return;
	if(!texturesLoaded)
		finishTextureLoad();

	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	enableWaterShader();
	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			chunks[j][i].drawWater();
		}
	}

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glEnable(GL_COLOR_MATERIAL);
}

void MapTile::drawObjects()
{
	if(!wmosLoaded)
		return;

	if(!ok)
		return;

	glEnable(GL_LIGHTING);
	for(int i = 0; i < nWMO; i++)
	{
		wmois[i].draw();
	}
}

void MapTile::drawObjectsSelect()
{
	if(!wmosLoaded)
		return;

	if(!ok)
		return;

	for(int i = 0; i < nWMO; i++)
	{
		wmois[i].drawSelect();
	}
}

void MapTile::drawSky()
{
	if(!wmosLoaded)
		return;

	if(!ok)
		return;

	for(int i = 0; i < nWMO; i++)
	{
		wmois[i].wmo->drawSkybox();
		if(gWorld->hadSky)
			break;
	}
}

/*
void MapTile::drawPortals()
{
	if (!ok) return;

	for (int i=0; i<nWMO; i++)
	{
		wmois[i].drawPortals();
	}
}
*/

void MapTile::drawModels()
{
	if(!ok)
		return;

	glEnable(GL_LIGHTING);
	for(int i = 0; i < nMDX; i++)
	{
		modelis[i].draw();
	}
}

void MapTile::drawModelsMapTile()
{
	if(!ok)
		return;

	for (int i = 0; i < nMDX; i++)
	{
		modelis[i].drawMapTile();
	}
}

void MapTile::drawModelsSelect()
{
	if(!ok)
		return;

	for(int i = 0; i < nMDX; i++)
	{
		modelis[i].drawSelect();
	}
}

void MapTile::drawTextures()
{
	if(!texturesLoaded)
		finishTextureLoad();

	if(!chunksLoaded)
	{
		while(!chunksLoaded)
			loadChunk();
	}
	
	float xOffset,yOffset;
	if(!ok)
		return;

	glPushMatrix();
	xOffset = xbase/CHUNKSIZE;
	yOffset = zbase/CHUNKSIZE;
	glTranslatef(xOffset, yOffset, 0);
	
	//glTranslatef(-8,-8,0);
	
	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			if(((i + 1 + xOffset) > gWorld->minX) && ((j + 1 + yOffset)>gWorld->minY) && ((i + xOffset) < gWorld->maxX) && ((j + yOffset) < gWorld->maxY))
				chunks[j][i].drawTextures();
		}
	}
	glPopMatrix();
}

int holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
int holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

bool isHole(int holes, int i, int j)
{
	return (holes & holetab_h[i] & holetab_v[j]) != 0;
}

int indexMapBuf(int x, int y)
{
	return ((y + 1)/2) * 9 + (y/2) * 8 + x;
}

void MapChunk::init(MapTile* maintile, MPQFile &f)
{	
	mt = maintile;

    f.seekRelative(4);
	char fcc[5];
	size_t size;
	f.read(&size, 4);

	// okay here we go ^_^
	size_t lastpos = f.getPos() + size;

	//char header[0x80];
	
	f.read(&header, 0x80);

	Flags = header.flags;
	areaID = header.areaid;
	
    zbase = header.zpos;
    xbase = header.xpos;
    ybase = header.ypos;

	px = header.ix;
	py = header.iy;

	int holes = header.holes;
	int chunkflags = header.flags;

	hasholes = (holes != 0);

	/*
	if (hasholes)
	{
		gLog("Holes: %d\n", holes);
		int k=1;
		for (int j=0; j<4; j++)
		{
			for (int i=0; i<4; i++)
			{
				gLog((holes & k)?"1":"0");
				k <<= 1;
			}
			gLog("\n");
		}
	}
	*/

	// correct the x and z values ^_^
	zbase = zbase * -1.0f + ZEROPOINT;
	xbase = xbase * -1.0f + ZEROPOINT;

	vmin = Vec3D( 9999999.0f, 9999999.0f, 9999999.0f);
	vmax = Vec3D(-9999999.0f, -9999999.0f, -9999999.0f);
	glGenTextures(3, alphamaps);
	
	while (f.getPos() < lastpos)
	{
		f.read(fcc, 4);
		f.read(&size, 4);

		flipcc(fcc);
		fcc[4] = 0;

		size_t nextpos = f.getPos() + size;

		if(!strcmp(fcc, "MCNR"))
		{
			nextpos = f.getPos() + 0x1C0; // size fix
			// normal vectors
			char nor[3];
			Vec3D* ttn = tn;
			for(int j = 0; j < 17; j++)
			{
				for(int i = 0; i < ((j%2)? 8 : 9); i++)
				{
					f.read(nor, 3);
					// order Z,X,Y ?
					//*ttn++ = Vec3D((float)nor[0]/127.0f, (float)nor[2]/127.0f, (float)nor[1]/127.0f);
					*ttn++ = Vec3D(-(float)nor[1]/127.0f, (float)nor[2]/127.0f, -(float)nor[0]/127.0f);
				}
			}
		}
		else if(!strcmp(fcc, "MCVT"))
		{
			Vec3D* ttv = tv;

			// vertices
			for(int j = 0; j < 17; j++)
			{
				for(int i = 0; i < ((j%2)? 8 : 9); i++)
				{
					float h, xpos, zpos;
					f.read(&h, 4);
					xpos = i * UNITSIZE;
					zpos = j * 0.5f * UNITSIZE;

					if(j%2)
                        xpos += UNITSIZE * 0.5f;

					Vec3D v = Vec3D(xbase + xpos, ybase + h, zbase + zpos);
					*ttv++ = v;
					if (v.y < vmin.y)
						vmin.y = v.y;
					if (v.y > vmax.y)
						vmax.y = v.y;
				}
			}

			vmin.x = xbase;
			vmin.z = zbase;
			vmax.x = xbase + 8 * UNITSIZE;
			vmax.z = zbase + 8 * UNITSIZE;
			r = (vmax - vmin).length() * 0.5f;

		}
		else if(!strcmp(fcc, "MCLY"))
		{
			// texture info
			nTextures = (int)size / 16;
			//gLog("=\n");
			for (int i = 0; i < nTextures; i++)
			{
				f.read(&tex[i], 4);
				f.read(&texFlags[i], 4);

				f.seekRelative(4);
				f.read(&effectID[i], 4);

				texFlags[i] &= ~0x100;

				if(texFlags[i] & 0x80)
                    animated[i] = texFlags[i];
				else
					animated[i] = 0;

				/*
				if (flags != 0)
				{
					gLog("[World of Warcraft Studio - Editor] - Texture layer flags: %x ", flags);
					int v = 0x80;
					for (int i=0; i<8; i++,v>>=1)
					{
						gLog("[World of Warcraft Studio - Editor] - %c%s", (flags&v)?'1':'-', i==3?" ":"");
					}

					gLog("[World of Warcraft Studio - Editor] - %s\n", mt->textures[tex].c_str());
				}
				*/

				/*
				if (mt->textures[tex]=="Tileset\\BurningStepps\\BurningSteppsLavatest02.blp")
				{
					gLog("[World of Warcraft Studio - Editor] - Lava tex:\t%d\t%d\t%d\n", unk[0], unk[1], unk[2]);
				}
				else
				{
					gLog("[World of Warcraft Studio - Editor] - ---- tex:\t%d\t%d\t%d\n", unk[0], unk[1], unk[2]);
				}
				*/

				textures[i] = video.textures.get(mt->textures[tex[i]]);
			}
		}
		else if(!strcmp(fcc, "MCSH"))
		{
			// shadow map 64 x 64
			unsigned char sbuf[64 * 64], *p, c[8];
			p = sbuf;
			for(int j = 0; j < 64; j++)
			{
				f.read(c, 8);
				for(int i = 0; i < 8; i++)
				{
					for(int b = 0x01; b != 0x100; b <<= 1)
					{
						*p++ = (c[i] & b) ? 85 : 0;
					}
				}
			}
			glGenTextures(1, &shadow);
			glBindTexture(GL_TEXTURE_2D, shadow);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, sbuf);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}
		else if(!strcmp(fcc, "MCAL"))
		{
			// alpha maps  64 x 64			
			if(nTextures>0)
			{
				//glGenTextures(3/*nTextures-1*/, alphamaps);
				for(int i = 0; i < nTextures - 1; i++)
				{
					glBindTexture(GL_TEXTURE_2D, alphamaps[i]);
					unsigned char* p;
					char* abuf = f.getPointer();
					p = amap[i];
					for(int j = 0; j < 63; j++)
					{
						for(int i = 0; i < 32; i++)
						{
							unsigned char c = *abuf++;
							if(i != 31)
							{
								*p++ = (unsigned char)((255 * ((int)(c & 0x0f)))/0x0f);
								*p++ = (unsigned char)((255 * ((int)(c & 0xf0)))/0xf0);
							}
							else
							{
								*p++ = (unsigned char)((255 * ((int)(c & 0x0f)))/0x0f);
								*p++ = (unsigned char)((255 * ((int)(c & 0x0f)))/0x0f);
							}
						}

					}
					memcpy(amap[i] + 63 * 64, amap[i] + 62 * 64, 64);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, amap[i]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					f.seekRelative(0x800);

					/*FILE *fid;
					char tfname[255];
					sprintf(tfname,"alpha %d Tile %d %d chunk %d %d.raw",i,mt->x,mt->z,px,py);
					fid=fopen(tfname,"wb");
					fwrite(amap[i],64,64,fid);
					fclose(fid);*/
				}
			}
			else
				// some MCAL chunks have incorrect sizes! :(
                continue;
		}
		else if(!strcmp(fcc, "MCLQ"))
		{
			// liquid / water level
			char fcc1[5];
			f.read(fcc1, 4);
			flipcc(fcc1);
			fcc1[4] = 0;
			if(!strcmp(fcc1, "MCSE"))
				haswater = false;
			else
			{
				haswater = true;
				f.seekRelative(-4);
				f.read(waterlevel, 8);//2 values - Lowest water Level, Highest Water Level

				if(waterlevel[1] > vmax.y)
					vmax.y = waterlevel[1];
				//if (waterlevel < vmin.y) haswater = false;

				//f.seekRelative(4);

				lq = new Liquid(8, 8, Vec3D(xbase, waterlevel[1], zbase));
				//lq->init(f);
				lq->initFromTerrain(f, chunkflags);

				/*
				// let's output some debug info! ( '-')b
				string lq = "";
				if (flags & 4) lq.append(" river");
				if (flags & 8) lq.append(" ocean");
				if (flags & 16) lq.append(" magma");
				if (flags & 32) lq.append(" slime?");
				gLog("[World of Warcraft Studio - Editor] - LQ%s (base:%f)\n", lq.c_str(), waterlevel);
				*/

			}
			// we're done here!
			break;
		}
		f.seek((int)nextpos);
	}

	// create vertex buffers
	glGenBuffersARB(1, &vertices);
	glGenBuffersARB(1, &normals);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tv, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tn, GL_STATIC_DRAW_ARB);

	if(hasholes)
		initStrip(holes);
	/*
	else {
		strip = gWorld->mapstrip;
		striplen = 16*18 + 7*2 + 8*2; //stripsize;
	}
	*/

	this->mt = mt;

	vcenter = (vmin + vmax) * 0.5f;

	deleted = false;
	nameID = addNameMapChunk(this);

	Vec3D* ttv = tm;

	// vertices
	for(int j = 0; j < 17; j++)
	{
		for(int i = 0; i < ((j%2)? 8 : 9); i++)
		{
			float h, xpos, zpos;
			f.read(&h, 4);
			xpos = i * 0.125f;
			zpos = j * 0.5f * 0.125f;
			if(j%2)
                 xpos += 0.125f * 0.5f;

			Vec3D v = Vec3D(xpos + px, zpos + py, -1);
			*ttv++ = v;
		}
	}

	float ShadowAmount;
	for(int j = 0; j < mapbufsize; j++)
	{
		//tm[j].z=tv[j].y;
		ShadowAmount = 1.0f - (-tn[j].x + tn[j].y - tn[j].z);
		if(ShadowAmount < 0)
			ShadowAmount = 0.0f;
		if(ShadowAmount > 1.0)
			ShadowAmount = 1.0f;
		ShadowAmount *= 0.5f;
		//ShadowAmount=0.2;
		ts[j].x = 0;
		ts[j].y = 0;
		ts[j].z = 0;
		ts[j].w = ShadowAmount;
	}

	glGenBuffersARB(1, &minimap);
	glGenBuffersARB(1, &minishadows);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, minimap);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tm, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, minishadows);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 4 * sizeof(float), ts, GL_STATIC_DRAW_ARB);
}

void MapChunk::loadTextures()
{
	return;
	for(int i = 0; i < nTextures; i++)
		textures[i] = video.textures.get(mt->textures[tex[i]]);
}

#define texDetail 8.0f

void SetAnim(int anim)
{
	if(anim)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();

		// note: this is ad hoc and probably completely wrong
		int spd = (anim & 0x08) | ((anim & 0x10) >> 2) | ((anim & 0x20) >> 4) | ((anim & 0x40) >> 6);
		int dir = anim & 0x07;
		const float texanimxtab[8] = {0, 1, 1, 1, 0, -1, -1, -1};
		const float texanimytab[8] = {1, 1, 0, -1, -1, -1, 0, 1};
		float fdx = -texanimxtab[dir], fdy = texanimytab[dir];

		int animspd = (int)(200.0f * 8.0f);
		float f = (((int)(gWorld->animtime * (spd/15.0f))) % animspd) / (float)animspd;
		glTranslatef(f * fdx, f * fdy, 0);
	}
}

void RemoveAnim(int anim)
{
	if(anim)
	{
        glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glActiveTextureARB(GL_TEXTURE1_ARB);
	}
}

#define	TEX_RANGE 62.0f/64.0f

void MapChunk::drawTextures()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if(nTextures > 0)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
	}

	SetAnim(animated[0]);
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(0.0f, texDetail);
	glVertex3f((float)px, py + 1.0f, -2.0f);	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f((float)px, (float)py, -2.0f);
	glTexCoord2f(texDetail, texDetail);
	glVertex3f((float)px + 1.0f, (float)py + 1.0f, -2.0f);	
	glTexCoord2f(texDetail, 0.0f);
	glVertex3f((float)px + 1.0f, (float)py, -2.0f);	
	glEnd();
	RemoveAnim(animated[0]);

	if(nTextures > 1)
	{
		//glDepthFunc(GL_EQUAL); // GL_LEQUAL is fine too...?
		//glDepthMask(GL_FALSE);
	}
	for(int i = 1; i < nTextures; i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, alphamaps[i-1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SetAnim(animated[i]);

		glBegin(GL_TRIANGLE_STRIP);	
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,texDetail, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,TEX_RANGE, 0.0f);
		glVertex3f(px + 1.0f, (float)py, -2.0f);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f((float)px, (float)py, -2.0f);		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texDetail, texDetail);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, TEX_RANGE, TEX_RANGE);
		glVertex3f(px + 1.0f, py + 1.0f, -2.0f);	
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, texDetail);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, TEX_RANGE);
		glVertex3f((float)px, py + 1.0f, -2.0f);	
		glEnd();

		RemoveAnim(animated[i]);
	}
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, minimap);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, minishadows);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	glDrawElements(GL_TRIANGLE_STRIP, stripsize2, GL_UNSIGNED_SHORT, gWorld->mapstrip2);
}

void MapChunk::initStrip(int holes)
{
	strip = new short[256]; // TODO: figure out exact length of strip needed
	short *s = strip;
	bool first = true;
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			if(!isHole(holes, x, y))
			{
				// draw tile here
				// this is ugly but sort of works
				int i = x * 2;
				int j = y * 4;
				for(int k = 0; k < 2; k++)
				{
					if(!first)
					{
						*s++ = indexMapBuf(i, j + k * 2);
					}
					else
						first = false;
					for(int l = 0; l < 3; l++)
					{
						*s++ = indexMapBuf(i + l, j + k * 2);
						*s++ = indexMapBuf(i + l, j + k * 2 + 2);
					}
					*s++ = indexMapBuf(i + 2, j + k * 2 + 2);
				}
			}
		}
	}
	striplen = (int)(s - strip);
}

void MapChunk::destroy()
{
	if(deleted)
		return;

	deleted = true;
	// unload alpha maps
	glDeleteTextures(3, alphamaps);
	// shadow maps, too
	glDeleteTextures(1, &shadow);

	// delete VBOs
	glDeleteBuffersARB(1, &vertices);
	glDeleteBuffersARB(1, &normals);

	if(hasholes)
		delete[] strip;

	if(haswater)
		delete lq;

	if(nameID != -1)
	{
		deleteName(nameID);
		nameID = -1;
	}
}

bool MapChunk::GetVertex(float x, float z, Vec3D* V)
{
	float xdiff, zdiff;

	xdiff=x-xbase;
	zdiff=z-zbase;
	
	int row, column;
	row = int(zdiff/(UNITSIZE * 0.5) + 0.5);
	column = int((xdiff-UNITSIZE * 0.5 * (row%2))/UNITSIZE + 0.5);
	if((row < 0) || (column < 0) || (row > 16)||(column>((row%2)? 8 : 9)))
		return false;

	*V = tv[17 * (row/2) + ((row%2)? 9 : 0 ) + column];
	return true;
}

unsigned short OddStrips[8 * 18];
unsigned short EvenStrips[8 * 18];
unsigned short LineStrip[32];

void CreateStrips()
{
	unsigned short Temp[18];
	int j;

	for(int i = 0; i < 8; i++)
	{
		OddStrips[i * 18 + 0] = i * 17 + 17;
		for(j = 0; j < 8; j++)
		{
			OddStrips[i * 18 + 2 * j + 1] =i * 17 + j;
			OddStrips[i * 18 + 2 * j + 2] =i * 17 + j + 9;
			EvenStrips[i * 18 + 2 * j] =i * 17 + 17 + j;
			EvenStrips[i * 18 + 2 *j + 1] = i * 17 + 9 + j;			
		}
		OddStrips[i * 18 + 17] = i * 17 + 8;
		EvenStrips[i * 18 + 16] = i * 17 + 17 + 8;
		EvenStrips[i * 18 + 17] = i * 17 + 8;
	}

	//Reverse the order whoops
	for(int i = 0; i < 8; i++)
	{
		for(j = 0; j < 18; j++)
			Temp[17 - j] = OddStrips[i * 18 + j];
		memcpy(&OddStrips[i * 18], Temp, sizeof(unsigned short) * 18);
		for(j = 0; j < 18; j++)
			Temp[17 - j] = EvenStrips[i * 18 + j];
		memcpy(&EvenStrips[i * 18], Temp, sizeof(unsigned short) * 18);

	}
	
	for(int i = 0; i < 32; i++)
	{
		if(i < 9)
			LineStrip[i] = i;
		else if(i < 17)
			LineStrip[i] = 8 + (i - 8) * 17;
		else if(i < 25)
			LineStrip[i] = 145 - (i - 15);
		else
			LineStrip[i] = (32 - i) * 17;
	}
}

void MapChunk::drawColor()
{
	
	if(!gWorld->frustum.intersects(vmin, vmax))
		return;

	float mydist = (gWorld->camera - vcenter).length() - r;
	//if (mydist > gWorld->mapdrawdistance2) return;
	if(mydist > gWorld->culldistance)
	{
		if(gWorld->uselowlod)
			this->drawNoDetail();

		return;
	}
	visible = true;

	if(!hasholes)
	{
		bool highres = gWorld->drawhighres;
		if(highres)
			highres = mydist < gWorld->highresdistance2;
		if(highres)
		{
			strip = gWorld->mapstrip2;
			striplen = stripsize2;
		}
		else
		{
			strip = gWorld->mapstrip;
			striplen = stripsize;
		}
	}

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);

	Vec3D Color;
	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0; i < striplen; i++)
	{
		HeightColor(tv[strip[i]].y, &Color);
		glColor3fv(&Color.x);
		glNormal3fv(&tn[strip[i]].x);
		glVertex3fv(&tv[strip[i]].x);
	}
	glEnd();
	//glEnable(GL_LIGHTING);
}

void MapChunk::drawPass(int anim)
{
	if(anim)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();

		// note: this is ad hoc and probably completely wrong
		int spd = (anim & 0x08) | ((anim & 0x10) >> 2) | ((anim & 0x20) >> 4) | ((anim & 0x40) >> 6);
		int dir = anim & 0x07;
		const float texanimxtab[8] = {0, 1, 1, 1, 0, -1, -1, -1};
		const float texanimytab[8] = {1, 1, 0, -1, -1, -1, 0, 1};
		float fdx = -texanimxtab[dir], fdy = texanimytab[dir];

		int animspd = (int)(200.0f * detail_size);
		float f = ( ((int)(gWorld->animtime * (spd/15.0f))) % animspd) / (float)animspd;
		glTranslatef(f * fdx, f * fdy, 0);
	}

	/*if(!hasholes)
		{
		glFrontFace(GL_CW);
		for(int i=0;i<8;i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, &OddStrips[i*18]);
			glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, &EvenStrips[i*18]);
		}
		glFrontFace(GL_CCW);
	}
	else*/
		glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);

	if(anim)
	{
        glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glActiveTextureARB(GL_TEXTURE1_ARB);
	}
}

void MapChunk::drawLines()
{
	if(!gWorld->frustum.intersects(vmin, vmax))
		return;

	float mydist = (gWorld->camera - vcenter).length() - r;
	//if (mydist > gWorld->mapdrawdistance2) return;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0f, 0.5f);
	glTranslatef(0.0f, 0.05f, 0.0f);
	glEnable (GL_LINE_SMOOTH);	
	glLineWidth(1.5);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	if((px != 15) && (py != 0))
		glDrawElements(GL_LINE_STRIP, 17, GL_UNSIGNED_SHORT, LineStrip);
	else if((px == 15) && (py == 0))
	{
		glColor4f(0.0, 1.0, 0.0f, 0.5f);
		glDrawElements(GL_LINE_STRIP, 17, GL_UNSIGNED_SHORT, LineStrip);
	}
	else if(px == 15)
	{
		glDrawElements(GL_LINE_STRIP, 9, GL_UNSIGNED_SHORT, LineStrip);
		glColor4f(0.0, 1.0, 0.0f, 0.5f);
		glDrawElements(GL_LINE_STRIP, 9, GL_UNSIGNED_SHORT, &LineStrip[8]);
	}
	else if(py == 0)
	{
		glColor4f(0.0, 1.0, 0.0f, 0.5f);
		glDrawElements(GL_LINE_STRIP, 9, GL_UNSIGNED_SHORT, LineStrip);
		glColor4f(1.0, 0.0, 0.0f, 0.5f);
		glDrawElements(GL_LINE_STRIP, 9, GL_UNSIGNED_SHORT, &LineStrip[8]);
	}
	
	glPopMatrix();

	glColor4f(1, 1, 1, 1);
}

void MapChunk::drawContour()
{
	if(!DrawMapContour)
		return;

	glColor4f(1, 1, 1, 1);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_ALPHA_TEST);
	if(Contour == 0)
		GenerateContourMap();

	glBindTexture(GL_TEXTURE_2D, Contour);
	
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGenfv(GL_S,GL_OBJECT_PLANE,CoordGen);
	
	drawPass(0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
}

extern nameEntry* Selection;
void MapChunk::draw()
{
	if(!gWorld->frustum.intersects(vmin, vmax))
		return;

	float mydist = (gWorld->camera - vcenter).length() - r;
	//if (mydist > gWorld->mapdrawdistance2) return;
	if (mydist > gWorld->culldistance + 75)
	{
		if(gWorld->uselowlod)
			this->drawNoDetail();

		return;
	}
	visible = true;

	if(!hasholes)
	{
		bool highres = gWorld->drawhighres;
		if(highres)
			highres = mydist < gWorld->highresdistance2;
		if(highres)
		{
			strip = gWorld->mapstrip2;
			striplen = stripsize2;
		}
		else
		{
			strip = gWorld->mapstrip;
			striplen = stripsize;
		}
	}

	// setup vertex buffers
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals);
	glNormalPointer(GL_FLOAT, 0, 0);
	// ASSUME: texture coordinates set up already

	// first pass: base texture
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	if(nameID == -1)
		nameID = addNameMapChunk(this);

	glPushName(nameID);
	if(nTextures == 0)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);

		glColor3f(1.0f, 1.0f, 1.0f);
		glDisable(GL_LIGHTING);
		glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);

		drawContour();
		return;
	}
	glEnable(GL_LIGHTING);
	drawPass(animated[0]);
	glPopName();

	if(nTextures > 1)
		//glDepthFunc(GL_EQUAL); // GL_LEQUAL is fine too...?
		glDepthMask(GL_FALSE);

	// additional passes: if required
	for(int i = 0; i < nTextures - 1; i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[i+1]);
		// this time, use blending:
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, alphamaps[i]);

		drawPass(animated[i + 1]);
	}

	if(nTextures > 1)
		//glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
	
	// shadow map
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	Vec3D shc = gWorld->skies->colorSet[WATER_COLOR_DARK] * 0.3f;
	//glColor4f(0,0,0,1);
	glColor4f(shc.x, shc.y, shc.z, 1);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, shadow);
	glEnable(GL_TEXTURE_2D);

	drawPass(0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	drawContour();
	
	if(drawFlags)
	{
		if(Flags&0x02)
		{
			glColor4f(1, 0, 0, 0.2f);
			drawPass(0);
		}

		if(Flags&0x04)
		{
			glColor4f(0, 0.5f, 1, 0.2f);
			drawPass(0);
		}

		if(Flags&0x08)
		{
			glColor4f(0, 0, 0.8f, 0.2f);
			drawPass(0);
		}
		if(Flags&0x10)
		{
			glColor4f(1, 0.5f, 0, 0.2f);
			drawPass(0);
		}
	}
	
	if(Selection && (Selection->type == ENTRY_MAPCHUNK) && (Selection->data.mapchunk == this))
	{
		int poly;
		poly = gWorld->getSelectTex();
		glColor4f(1, 0.3f, 0.3f, 0.2f);

		glPushMatrix();
		glDisable(GL_CULL_FACE);
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLES);
		glVertex3fv(&(tv[strip[poly]].x));
		glVertex3fv(&(tv[strip[poly + 1]].x));
		glVertex3fv(&(tv[strip[poly + 2]].x));
		glEnd();		
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(true);

		glPopMatrix();
	}

	glColor4f(1, 1, 1, 1);
	
	glEnable(GL_LIGHTING);
	glColor4f(1, 1, 1, 1);

	/*
	//////////////////////////////////
	// debugging tile flags:
	GLfloat tcols[8][4] = {	{1,1,1,1},
		{1,0,0,1}, {1, 0.5f, 0, 1}, {1, 1, 0, 1},
		{0,1,0,1}, {0,1,1,1}, {0,0,1,1}, {0.8f, 0, 1,1}
	};
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(xbase, ybase, zbase);
	for (int i=0; i<8; i++)
	{
		int v = 1 << (7-i);
		for (int j=0; j<4; j++)
		{
			if (animated[j] & v)
			{
				glBegin(GL_TRIANGLES);
				glColor4fv(tcols[i]);

				glVertex3f(i*2.0f, 2.0f, j*2.0f);
				glVertex3f(i*2.0f+1.0f, 2.0f, j*2.0f);
				glVertex3f(i*2.0f+0.5f, 4.0f, j*2.0f);

				glEnd();
			}
		}
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glColor4f(1,1,1,1);
	glPopMatrix();
	*/
}

void MapChunk::drawNoDetail()
{
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glColor3fv(gWorld->skies->colorSet[FOG_COLOR]);
	//glColor3f(1,0,0);
	//glDisable(GL_FOG);

	// low detail version
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawElements(GL_TRIANGLE_STRIP, stripsize, GL_UNSIGNED_SHORT, gWorld->mapstrip);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColor4f(1, 1, 1, 1);
	//glEnable(GL_FOG);

	glEnable(GL_LIGHTING);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
}

void MapChunk::drawSelect()
{
	int ErrorNum;

	if(!gWorld->frustum.intersects(vmin, vmax))
		return;

	float mydist = (gWorld->camera - vcenter).length() - r;
	//if (mydist > gWorld->mapdrawdistance2) return;
	if(mydist > gWorld->culldistance)
		return;

	if(nameID == -1)
		nameID = addNameMapChunk(this);

	glPushName(nameID);
	ErrorNum = glGetError();
	if(ErrorNum)
		gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n", ErrorNum);

	if(!hasholes)
	{
		bool highres = gWorld->drawhighres;
		if(highres)
			highres = mydist < gWorld->highresdistance2;
		if (highres)
		{
			strip = gWorld->mapstrip2;
			striplen = stripsize2;
		}
		else
		{
			strip = gWorld->mapstrip;
			striplen = stripsize;
		}
	}


	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0; i < striplen; i++)
		glVertex3fv(&(tv[strip[i]].x));

	glEnd();

	glPopName();
}

void MapChunk::drawSelect2()
{
	int ErrorNum;

	float mydist = (gWorld->camera - vcenter).length() - r;
	//if (mydist > gWorld->mapdrawdistance2) return;
	if(mydist > gWorld->culldistance)
		return;

	if(nameID == -1)
		nameID=addNameMapChunk(this);

	glPushName(nameID);
	ErrorNum = glGetError();
	if(ErrorNum)
		gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n", ErrorNum);

	if(!hasholes)
	{
		bool highres = gWorld->drawhighres;
		if(highres)
			highres = mydist < gWorld->highresdistance2;
		if (highres)
		{
			strip = gWorld->mapstrip2;
			striplen = stripsize2;
		}
		else
		{
			strip = gWorld->mapstrip;
			striplen = stripsize;
		}
	}

	glDisable(GL_CULL_FACE);
	for(int i = 0; i < striplen - 2; i++)
	{
		glPushName(i);
		glBegin(GL_TRIANGLES);
		glVertex3fv(&(tv[strip[i]].x));
		glVertex3fv(&(tv[strip[i + 1]].x));
		glVertex3fv(&(tv[strip[i + 2]].x));
		glEnd();
		glPopName();	
	}
	glEnable(GL_CULL_FACE);

	glPopName();
}

void MapChunk::getSelectionCoord(float* x,float* z)
{
	int Poly = gWorld->getSelectTex();
	*x = (tv[strip[Poly]].x + tv[strip[Poly + 1]].x + tv[strip[Poly + 2]].x)/3;
	*z = (tv[strip[Poly]].z + tv[strip[Poly + 1]].z + tv[strip[Poly + 2]].z)/3;
}

float MapChunk::getSelectionHeight()
{
	int Poly = gWorld->getSelectTex();

	return (tv[strip[Poly]].y + tv[strip[Poly + 1]].y + tv[strip[Poly + 2]].y)/3;
}

void MapChunk::recalcNorms()
{
	Vec3D P1, P2, P3, P4;
	Vec3D Norm, N1, N2, N3, N4, D;

	if(Changed == false)
		return;

	Changed = false;

	for(int i = 0; i < mapbufsize; i++)
	{
		if(!gWorld->GetVertex(tv[i].x - UNITSIZE * 0.5f, tv[i].z - UNITSIZE * 0.5f, &P1))
		{
			P1.x = tv[i].x - UNITSIZE * 0.5f;
			P1.y = tv[i].y;
			P1.z = tv[i].z - UNITSIZE * 0.5f;
		}

		if(!gWorld->GetVertex(tv[i].x + UNITSIZE * 0.5f, tv[i].z - UNITSIZE * 0.5f, &P2))
		{
			P2.x = tv[i].x + UNITSIZE * 0.5f;
			P2.y = tv[i].y;
			P2.z = tv[i].z - UNITSIZE * 0.5f;
		}

		if(!gWorld->GetVertex(tv[i].x + UNITSIZE * 0.5f, tv[i].z + UNITSIZE * 0.5f, &P3))
		{
			P3.x = tv[i].x + UNITSIZE * 0.5f;
			P3.y = tv[i].y;
			P3.z = tv[i].z + UNITSIZE * 0.5f;
		}

		if(!gWorld->GetVertex(tv[i].x - UNITSIZE * 0.5f, tv[i].z + UNITSIZE * 0.5f, &P4))
		{
			P4.x = tv[i].x - UNITSIZE * 0.5f;
			P4.y = tv[i].y;
			P4.z = tv[i].z + UNITSIZE * 0.5f;
		}

		N1 = (P2 - tv[i])%(P1 - tv[i]);
		N2 = (P3 - tv[i])%(P2 - tv[i]);
		N3 = (P4 - tv[i])%(P3 - tv[i]);
		N4 = (P1 - tv[i])%(P4 - tv[i]);

		Norm = N1 + N2 + N3 + N4;
		Norm.normalize();
		tn[i] = Norm;
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tn, GL_STATIC_DRAW_ARB);

	float ShadowAmount;
	for(int j = 0; j < mapbufsize; j++)
	{
		//tm[j].z=tv[j].y;
		ShadowAmount = 1.0f -(-tn[j].x + tn[j].y - tn[j].z);
		if(ShadowAmount < 0)
			ShadowAmount = 0;
		if(ShadowAmount > 1.0)
			ShadowAmount = 1.0f;
		ShadowAmount *= 0.5f;
		//ShadowAmount=0.2;
		ts[j].x = 0;
		ts[j].y = 0;
		ts[j].z = 0;
		ts[j].w = ShadowAmount;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, minishadows);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 4 * sizeof(float), ts, GL_STATIC_DRAW_ARB);
}

void MapChunk::changeTerrain(float x, float z, float change, float radius, int BrushType)
{
	float dist, xdiff, zdiff;
	
	Changed = false;

	xdiff = xbase - x + CHUNKSIZE/2;
	zdiff = zbase - z + CHUNKSIZE/2;
	dist = sqrt(xdiff * xdiff + zdiff * zdiff);

	if(dist > (radius + MAPCHUNK_RADIUS))
		return;

	vmin.y =  9999999.0f;
	vmax.y = -9999999.0f;

	for(int i = 0; i < mapbufsize; i++)
	{
		xdiff = tv[i].x - x;
		zdiff = tv[i].z - z;

		dist = sqrt(xdiff * xdiff + zdiff * zdiff);

		if(dist < radius)
		{
			if(BrushType == 0)//Flat
				tv[i].y += change;
			else if(BrushType == 1)//Linear
				tv[i].y += change * (1.0f - dist/radius);
			else if(BrushType == 2)//Smooth
				tv[i].y += change/(1.0f + dist/radius);

			Changed = true;
		}
		
		if(tv[i].y < vmin.y)
			vmin.y = tv[i].y;
		if(tv[i].y > vmax.y)
			vmax.y = tv[i].y;
	}

	if(Changed)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tv, GL_STATIC_DRAW_ARB);
	}
}

void MapChunk::flattenTerrain(float x, float z, float h, float remain, float radius, int BrushType)
{
	float dist, xdiff, zdiff, nremain;
	Changed = false;

	xdiff = xbase - x + CHUNKSIZE/2;
	zdiff = zbase - z + CHUNKSIZE/2;
	dist = sqrt(xdiff * xdiff + zdiff * zdiff);

	if(dist > (radius + MAPCHUNK_RADIUS))
		return;

	vmin.y =  9999999.0f;
	vmax.y = -9999999.0f;

	for(int i = 0; i < mapbufsize; i++)
	{
		xdiff = tv[i].x - x;
		zdiff = tv[i].z - z;

		dist = sqrt(xdiff * xdiff + zdiff * zdiff);

		if(dist < radius)
		{
			if(BrushType == 0) // Flat
			{
				tv[i].y = remain * tv[i].y + (1 - remain) * h;
			}
			else if(BrushType == 1) // Linear
			{
				nremain = 1 - (1 - remain) * (1 - dist/radius);
				tv[i].y = nremain * tv[i].y + (1 - nremain) * h;
			}
			else if(BrushType == 2)//Smooth
			{
				nremain = 1.0f - pow(1.0f - remain, (1.0f + dist/radius));
				tv[i].y = nremain * tv[i].y + (1 - nremain) * h;
			}

			Changed = true;
		}
		
		if(tv[i].y < vmin.y)
			vmin.y = tv[i].y;
		if(tv[i].y > vmax.y)
			vmax.y = tv[i].y;
	}

	if(Changed)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tv, GL_STATIC_DRAW_ARB);
	}
}

void MapChunk::blurTerrain(float x, float z, float remain, float radius, int BrushType)
{
	float dist, dist2, xdiff, zdiff, nremain;
	Changed = false;

	xdiff = xbase - x + CHUNKSIZE/2;
	zdiff = zbase - z + CHUNKSIZE/2;
	dist = sqrt(xdiff * xdiff + zdiff * zdiff);

	if(dist > (radius + MAPCHUNK_RADIUS))
		return;

	vmin.y =  9999999.0f;
	vmax.y = -9999999.0f;

	for(int i = 0; i < mapbufsize; i++)
	{
		xdiff = tv[i].x - x;
		zdiff = tv[i].z - z;

		dist = sqrt(xdiff * xdiff + zdiff * zdiff);

		if(dist < radius)
		{
			float TotalHeight;
			float TotalWeight;
			float tx, tz, h;
			Vec3D TempVec;
			int Rad;
			//Calculate Average
			Rad = (int)(radius/UNITSIZE);

			TotalHeight = 0;
			TotalWeight = 0;
			for(int j =- Rad * 2; j <= Rad * 2; j++)
			{
				tz = z + j * UNITSIZE/2;
				for(int k =- Rad; k <= Rad; k++)
				{
					tx = x + k * UNITSIZE + (j%2) * UNITSIZE/2.0f;
					xdiff = tx - tv[i].x;
					zdiff = tz - tv[i].z;
					dist2 = sqrt(xdiff * xdiff + zdiff * zdiff);
					if(dist2 > radius)
						continue;

					gWorld->GetVertex(tx, tz, &TempVec);
					TotalHeight += (1.0f - dist2/radius) * TempVec.y;
					TotalWeight += (1.0f - dist2/radius);
				}
			}

			h = TotalHeight/TotalWeight;

			if(BrushType == 0)//Flat
				tv[i].y = remain * tv[i].y + (1 - remain) * h;
			else if(BrushType == 1)//Linear
			{
				nremain = 1 - ( 1 - remain) * (1 - dist/radius);
				tv[i].y = nremain * tv[i].y + (1 - nremain) * h;
			}
			else if(BrushType == 2)//Smooth
			{
				nremain = 1.0f - pow(1.0f - remain, (1.0f + dist/radius));
				tv[i].y = nremain * tv[i].y + (1 - nremain) * h;
			}

			Changed = true;
		}
		
		if(tv[i].y < vmin.y)
			vmin.y = tv[i].y;
		if(tv[i].y > vmax.y)
			vmax.y = tv[i].y;
	}

	if(Changed)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mapbufsize * 3 * sizeof(float), tv, GL_STATIC_DRAW_ARB);
	}
}

/* The correct way to do everything
Visibility = (1-Alpha above)*Alpha

Objective is Visibility = level

if (not bottom texture)
	New Alpha = Pressure*Level+(1-Pressure)*Alpha;
	New Alpha Above = (1-Pressure)*Alpha Above;
else Bottom Texture 
	New Alpha Above = Pressure*(1-Level)+(1-Pressure)*Alpha Above

For bottom texture with multiple above textures

For 2 textures above
x,y = current alphas
u,v = target alphas
v=sqrt((1-level)*y/x)
u=(1-level)/v

For 3 textures above
x,y,z = current alphas
u,v,w = target alphas
L=(1-Level)
u=pow(L*x*x/(y*y),1.0f/3.0f)
w=sqrt(L*z/(u*y))
*/

void MapChunk::eraseTextures()
{
	nTextures = 0;
}

int MapChunk::addTexture(GLuint texture)
{
	int texLevel = -1;
	if(nTextures < 4)
	{
		texLevel = nTextures;
		nTextures++;
		textures[texLevel] = texture;
		animated[texLevel] = 0;
		texFlags[texLevel] = 0;
		effectID[texLevel] = 0;//findEffectID(video.textures.items[texture]->name.c_str());
		if(texLevel != 0)
		{
			if(alphamaps[texLevel - 1] < 1)
			{
				gLog("[World of Warcraft Studio - Editor] - Alpha Map has invalid texture binding\n");
				nTextures--;
				return -1;
			}

			memset(amap[texLevel - 1], 0, 64 * 64);
			glBindTexture(GL_TEXTURE_2D, alphamaps[texLevel - 1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, amap[texLevel - 1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	}
	return texLevel;
}

bool MapChunk::paintTexture(float x, float z, brush* Brush, float strength, float pressure, int texture)
{
	float zPos, xPos, change, xdiff, zdiff, dist, radius;

	int texLevel = -1, i, j;

	radius = Brush->getRadius();

	xdiff = xbase - x + CHUNKSIZE/2;
	zdiff = zbase - z + CHUNKSIZE/2;
	dist = sqrt(xdiff * xdiff + zdiff * zdiff);

	if(dist > (radius + MAPCHUNK_RADIUS))
		return true;

	//First Lets find out do we have the texture already
	for(i = 0; i < nTextures; i++)
		if(textures[i] == texture)
			texLevel = i;

	//At Max Limit
	if((texLevel == -1) && (nTextures == 4))
		return false;
	
	//Only 1 layer and its that layer
	if((texLevel != -1) && (nTextures == 1))
		return true;
	
	change = CHUNKSIZE/62.0f;
	zPos = zbase;

	int texAbove;
	float target, tarAbove, tPressure;
	texAbove = nTextures - texLevel - 1;

	for(j = 0; j < 63; j++)
	{
		xPos = xbase;
		for(i = 0; i < 63; i++)
		{
			xdiff = xPos - x;
			zdiff = zPos - z;
			dist = abs(sqrt(xdiff * xdiff + zdiff * zdiff));
			
			if(dist > radius)
			{
				xPos += change;
				continue;
			}

			if(texLevel == -1)
			{
				texLevel = addTexture(texture);
				if(texLevel == 0)
					return true;
				if(texLevel == -1)
					return false;
			}
			
			target = strength;
			tarAbove = 1 - target;
			
			tPressure = pressure * Brush->getValue(dist);
			if(texLevel > 0)
				amap[texLevel - 1][i + j * 64]=(unsigned char)max(min((1 - tPressure) * ((float)amap[texLevel - 1][i + j * 64]) + tPressure * target + 0.5f , 255.0f), 0.0f);
			for(int k = texLevel; k < nTextures - 1; k++)
				amap[k][i + j * 64]=(unsigned char)max(min((1 - tPressure) * ((float)amap[k][i + j * 64] ) + tPressure * tarAbove + 0.5f, 255.0f), 0.0f);
/*			if(texAbove==1)
				amap[texLevel][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel][i+j*64] ) + tPressure*tarAAbove + 0.5f ,255.0f) , 0.0f);
			else if(texAbove==2)
			{
				/*For 2 textures above
				x,y = current alphas
				u,v = target alphas
				v=sqrt((1-level)*y/x)
				u=(1-level)/v*/
/*
				v=sqrt(tarAAbove*(float)amap[texLevel+1][i+j*64]/(float)amap[texLevel][i+j*64]);
				u=tarAAbove/v;
				amap[texLevel][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel][i+j*64] ) + tPressure*u + 0.5f, 255.0f) , 0.0f);
				amap[texLevel+1][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel+1][i+j*64] ) + tPressure*v + 0.5f , 255.0f) , 0.0f);
			}
			/*else if(texAbove==3)
			{
				/*For 3 textures above
				x,y,z = current alphas
				u,v,w = target alphas
				L=(1-Level)
				u=pow(L*x*x/(y*y),1.0f/3.0f)
				w=sqrt(L*z/(u*y))*/
			/*	u=pow(tarAAbove*(float)(amap[texLevel][i+j*64]*amap[texLevel][i+j*64])/(float)(amap[texLevel+1][i+j*64]*amap[texLevel+1][i+j*64]),1.0f/3.0f);
				w=sqrt(tarAAbove*(float)(amap[texLevel+2][i+j*64])/(u*(float)amap[texLevel+1][i+j*64]));
				v=tarAAbove/(u*w);

				amap[texLevel][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel][i+j*64] ) + tPressure*u + 0.5f ,255.0f) , 0.0f);
				amap[texLevel+1][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel+1][i+j*64] ) + tPressure*v + 0.5f ,255.0f) , 0.0f);
				amap[texLevel+2][i+j*64]=(unsigned char)max( min( (1-tPressure)*( (float)amap[texLevel+2][i+j*64] ) + tPressure*w + 0.5f ,255.0f) , 0.0f);
			}*/

			xPos += change;
		}
		zPos += change;
	}

	if(texLevel == -1)
		return false;

	for(int j = texLevel; j < nTextures - 1; j++)
	{
		if(j > 2)
		{
			gLog("[World of Warcraft Studio - Editor] - WTF how did you get here???");
			continue;
		}
		glBindTexture(GL_TEXTURE_2D, alphamaps[j]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, amap[j]);
	}

	if(texLevel != 0)
	{
		glBindTexture(GL_TEXTURE_2D, alphamaps[texLevel - 1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, amap[texLevel - 1]);
	}

	return true;
}

void MapChunk::drawWater()
{
	// TODO: figure out how water really works

	/*
	
	// (fake) WATER
	if (haswater) {
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, gWorld->water);

		const float wr = 1.0f;

		//glDepthMask(GL_FALSE);
		glBegin(GL_QUADS);
		
		//glColor4f(1.0f,1.0f,1.0f,0.95f);
		glColor4f(1,1,1,1); // fuck you alpha blending
		
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		glVertex3f(xbase, waterlevel, zbase);
		glTexCoord2f(0,wr);
		glVertex3f(xbase, waterlevel, zbase+CHUNKSIZE);
		glTexCoord2f(wr,wr);
		glVertex3f(xbase+CHUNKSIZE, waterlevel, zbase+CHUNKSIZE);
		glTexCoord2f(wr,0);
		glVertex3f(xbase+CHUNKSIZE, waterlevel, zbase);
		glEnd();

		//glColor4f(1,1,1,1);
		//glDepthMask(GL_TRUE);

	}

	*/

	if(haswater)
		lq->draw();
}

void MapNode::draw()
{
	/*if (!gWorld->frustum.intersects(vmin,vmax)) return;
	for (int i=0; i<4; i++) children[i]->draw();*/
}

void MapNode::drawSelect()
{
	//if (!gWorld->frustum.intersects(vmin,vmax)) return;
	//for (int i=0; i<4; i++) children[i]->drawSelect();
}

void MapNode::drawColor()
{
	if(!gWorld->frustum.intersects(vmin, vmax))
		return;

	for(int i = 0; i < 4; i++)
		children[i]->drawColor();
}

void MapNode::setup(MapTile* t)
{
	vmin = Vec3D( 9999999.0f, 9999999.0f, 9999999.0f);
	vmax = Vec3D(-9999999.0f, -9999999.0f, -9999999.0f);
	mt = t;
	if(size == 2)
	{
		// children will be mapchunks
		children[0] = &(mt->chunks[py][px]);
		children[1] = &(mt->chunks[py][px + 1]);
		children[2] = &(mt->chunks[py+1][px]);
		children[3] = &(mt->chunks[py+1][px + 1]);
	}
	else
	{
		int half = size / 2;
		children[0] = new MapNode(px, py, half);
		children[1] = new MapNode(px + half, py, half);
		children[2] = new MapNode(px, py + half, half);
		children[3] = new MapNode(px + half, py + half, half);
		for(int i = 0; i < 4; i++)
		{
			children[i]->setup(mt);
		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(children[i]->vmin.x < vmin.x)
			vmin.x = children[i]->vmin.x;
		if(children[i]->vmin.y < vmin.y)
			vmin.y = children[i]->vmin.y;
		if(children[i]->vmin.z < vmin.z)
			vmin.z = children[i]->vmin.z;
		if(children[i]->vmax.x > vmax.x)
			vmax.x = children[i]->vmax.x;
		if(children[i]->vmax.y > vmax.y)
			vmax.y = children[i]->vmax.y;
		if(children[i]->vmax.z > vmax.z)
			vmax.z = children[i]->vmax.z;
	}
}

void MapNode::cleanup()
{
	if(size > 2)
	{
		for(int i = 0; i < 4; i++)
		{
			children[i]->cleanup();
			delete children[i];
		}
	}
}

MapChunk* MapTile::getChunk(unsigned int x, unsigned int z)
{
	assert(x < 16 && z < 16);
	return &chunks[z][x];
}

char roundc(float a)
{
	if(a < 0)
		a -= 0.5f;
	if(a > 0)
		a += 0.5f;
	if(a < -127)
		a = -127;
	else if(a > 127)
		a = 127;

	return (char)a;
}

bool pointInside(Vec3D point, Vec3D extents[2])
{
	if((point.x >= extents[0].x) && (point.z >= extents[0].z) && (point.x <= extents[1].x) && (point.z <= extents[1].z))
		return true;

	return false;
}

bool checkInside(Vec3D extentA[2], Vec3D extentB[2])
{
	if(pointInside(extentA[0], extentB))
		return true;
	if(pointInside(extentA[1], extentB))
		return true;
	if(pointInside(extentB[0], extentA))
		return true;
	if(pointInside(extentB[1], extentA))
		return true;

	return false;
}

void MapTile::saveTile()
{
	MHDR MapHeader;
	MHDR newMapHeader;
	MCIN MCINs[256];
	MapChunkHeader ChunkHeader[256];
	int NewDoodadRefTot;
	unsigned int ChunkDoodadRef[2048];
	unsigned int NumDoodadRefs;
	unsigned int ChunkWMORef[1024];
	unsigned int NumWMORefs;
	int i;

	MPQFile f(fname.c_str());

	f.seek(0x14);
	f.read(&MapHeader, sizeof(MHDR));
	f.seek(0x14);
	f.read(&newMapHeader, sizeof(MHDR));
	
	f.seek(0x5c);
	f.read(MCINs, 256 * sizeof(MCIN));
	
	NewDoodadRefTot = 0;

	unsigned int NewFileSize = (unsigned int)(2 * f.getSize());
	char* Buffer;

	Buffer = new char[NewFileSize];
	memset(Buffer, 0, NewFileSize);

	memcpy(Buffer, f.getBuffer(), 0x14 + MapHeader.MTEX_Offset + 4);

	//Recreate the texture data
	map<GLuint, string>	texturesUsed;
	map<GLuint, int>	textureOffsets;
	string AllTextures;
	char* allTextures;
	int texSize;
	for(i = 0; i < 256; i++)
		for(int j = 0; j < chunks[i/16][i%16].nTextures; j++)
			texturesUsed[chunks[i/16][i%16].textures[j]] = video.textures.items[chunks[i/16][i%16].textures[j]]->name;

	i = 0;
	for(map<GLuint, string>::iterator texName = texturesUsed.begin(); texName != texturesUsed.end(); texName++)
	{
		char wtf[512];
		strcpy_s(wtf, texName->second.c_str());
		if(texName == texturesUsed.begin())
			AllTextures = texName->second;			
		else			
			AllTextures = AllTextures + "$" + texName->second;

		textureOffsets[texName->first] = i;
		i++;
	}

	texSize = AllTextures.size() + 1;
	allTextures = new char[texSize];
	strcpy(allTextures, AllTextures.c_str());
	for(i = 0; i < texSize; i++)
		if(allTextures[i] == '$')
			allTextures[i] = 0;
	
	int Change = 0, lChange;
	
	f.seek(0x14 + MapHeader.MTEX_Offset + 4);
	f.read(&Change, sizeof(int));

	memcpy(Buffer + 0x14 + MapHeader.MTEX_Offset + 4, &texSize, 4);
	memcpy(Buffer + 0x14 + MapHeader.MTEX_Offset + 8, allTextures, texSize);
	Change = texSize - Change;

	newMapHeader.MMDX_Offset += Change;
	newMapHeader.MMID_Offset += Change;
	newMapHeader.MWMO_Offset += Change;
	newMapHeader.MWID_Offset +=  Change;
	newMapHeader.MDDF_Offset += Change;
	newMapHeader.MODF_Offset += Change;

	memcpy(Buffer + 0x14 + newMapHeader.MMDX_Offset, f.getBuffer() + 0x14 + MapHeader.MMDX_Offset, MCINs[0].offset - (0x14 + MapHeader.MMDX_Offset));
	
	//Time to resave the doodads
	MDDF TempDDF;

	for(i = 0; i < nMDX; i++)
	{
		TempDDF.scale = (int)(modelis[i].sc * 1024);
		//TempDDF.flags=0;
		TempDDF.uniqueID = modelis[i].d1;
		TempDDF.nameID = modelis[i].modelID;
		for(int j = 0; j < 3; j++)
		{
			TempDDF.pos[j] = modelis[i].pos[j];
			TempDDF.rot[j] = modelis[i].dir[j];
		}
		memcpy(0x14 + Buffer + newMapHeader.MDDF_Offset + 8 + sizeof(MDDF) * i, &TempDDF, sizeof(MDDF));
	}

	for(i = 0; i < 256; i++)
	{
		gLog("[World of Warcraft Studio - Editor] - Saving Chunk %d (%d,%d)\n", i, i/16, i%16);
		f.seek(MCINs[i].offset + 8);
		f.read(&ChunkHeader[i], sizeof(MapChunkHeader));

		float ChunkX, ChunkZ, XDiff, ZDiff, Dist;

		ChunkX= 32 * 533.3333f - (ChunkHeader[i].xpos) + TILESIZE/32.0f;
		ChunkZ= 32 * 533.3333f - (ChunkHeader[i].zpos) + TILESIZE/32.0f;

		NumDoodadRefs = 0;

		Vec3D ChunkExtents[2];
		ChunkExtents[0] = Vec3D(-TILESIZE/32.0f, 0, -TILESIZE/32.0f);
		ChunkExtents[1] = Vec3D(TILESIZE/32.0f, 0, TILESIZE/32.0f);

		for(int j = 0; j < nMDX; j++)
		{
			XDiff = modelis[j].pos[0] - ChunkX;
			ZDiff = modelis[j].pos[2] - ChunkZ;

			Dist = sqrt(XDiff * XDiff + ZDiff * ZDiff) - modelis[j].sc * modelis[j].model->rad;
			

			if(Dist <= 0)//First simple check to see if the center is close enough
			{
				if(NumDoodadRefs == 2048)
				{
					gLog("[World of Warcraft Studio - Editor] - ERROR: Trying to add more than 2048 Doodads to a chunk\n");
					break;
				}

				ChunkDoodadRef[NumDoodadRefs] = j;
				NumDoodadRefs++;
			}
			else//Next after that check is verified we've eliminated the special case of the second check which is to check if the circles closest point is within the map chunk extents
			{
				Vec3D Closest = Vec3D(XDiff, 0, ZDiff);
				Closest.normalize();
				Closest *= Dist;

				if(pointInside(Closest, ChunkExtents))
				{
					if(NumDoodadRefs == 2048)
					{
						gLog("[World of Warcraft Studio - Editor] - ERROR: Trying to add more than 2048 Doodads to a chunk\n");
						break;
					}

					ChunkDoodadRef[NumDoodadRefs] = j;
					NumDoodadRefs++;
				}
			}			
		}

		ChunkExtents[0].x += ChunkX;
		ChunkExtents[0].z += ChunkZ;
		ChunkExtents[1].x += ChunkX;
		ChunkExtents[1].z += ChunkZ;
		NumWMORefs = 0;
		for(int j = 0; j < nWMO; j++)
		{
			Vec3D  WMOExtents[2];
			Matrix Rot;				

			Rot.transpose();
			Rot.unit();
			Rot.rotate(wmois[j].dir);
			Rot.m[0][3] += wmois[j].pos.x;
			Rot.m[1][3] += wmois[j].pos.y;
			Rot.m[2][3] += wmois[j].pos.z;

			for(int k = 0; k < wmois[j].wmo->nGroups; k++)
			{
				WMOExtents[0] = Rot * wmois[j].wmo->groups[k].vmin;
				WMOExtents[1] = Rot * wmois[j].wmo->groups[k].vmax;

				float tswap;
				if(WMOExtents[0].x > WMOExtents[1].x)
				{
					tswap = WMOExtents[0].x;
					WMOExtents[0].x = WMOExtents[1].x;
					WMOExtents[1].x = WMOExtents[0].x;
				}

				if(WMOExtents[0].y > WMOExtents[1].y)
				{
					tswap = WMOExtents[0].y;
					WMOExtents[0].y = WMOExtents[1].y;
					WMOExtents[1].y = WMOExtents[0].y;
				}

				if(WMOExtents[0].z > WMOExtents[1].z)
				{
					tswap = WMOExtents[0].z;
					WMOExtents[0].z = WMOExtents[1].z;
					WMOExtents[1].z = WMOExtents[0].z;
				}

				//Do these extents overlap?
				if(checkInside(ChunkExtents, wmois[j].extents))
				{
					ChunkWMORef[NumWMORefs] = j;
					NumWMORefs++;
					break;
				}		
			}
		}

		int Temp;
		unsigned int MCNKSize =* ((unsigned int*)(f.getBuffer() + MCINs[i].offset + 4));

		lChange = 0;

		memcpy(Buffer + Change + MCINs[i].offset, f.getBuffer() + MCINs[i].offset, ChunkHeader[i].ofsRefs + 4);

		float* vecHeight;
		char* Normals;
		vecHeight = (float*)(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsHeight + 8);
		Normals = (Buffer + Change + MCINs[i].offset+ChunkHeader[i].ofsNormal + 8);
		ChunkHeader[i].ypos = chunks[i/16][i%16].tv[0].y;
		for(int j = 0; j < mapbufsize; j++)
		{
			vecHeight[j] = chunks[i/16][i%16].tv[j].y-ChunkHeader[i].ypos;
			Normals[j * 3] = (char)roundc(-chunks[i/16][i%16].tn[j].z * 127);
			Normals[j * 3 + 1] = (char)roundc(-chunks[i/16][i%16].tn[j].x * 127);
			Normals[j * 3 + 2] = (char)roundc(chunks[i/16][i%16].tn[j].y * 127);
		}

		/*gLog("Chunk %d (%d, %d)\n",i,i/16,i%16);
		gLog("[World of Warcraft Studio - Editor] - Textures: %d\n",ChunkHeader[i].nLayers);
		gLog("[World of Warcraft Studio - Editor] - \tS1: %60d\tS2: %60d\n",ChunkHeader[i].s1,ChunkHeader[i].s2);
		gLog("[World of Warcraft Studio - Editor] - \tD1: %80d\tD2: %80d\n",ChunkHeader[i].d1,ChunkHeader[i].d2);
		gLog("[World of Warcraft Studio - Editor] - \tD3: %80d",ChunkHeader[i].d3);*/

		ChunkHeader[i].s1 = 0;
		ChunkHeader[i].s2 = 0;
		ChunkHeader[i].d1 = 0;
		ChunkHeader[i].d2 = 0;
		ChunkHeader[i].d3 = 0;

		//Recreating Layer Defintions
		Temp = sizeof(MCLY) * chunks[i/16][i%16].nTextures; //Size of the new layer defination
		memcpy(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsLayer + 4, &Temp, sizeof(int));
		
		for(int j  =0; j < chunks[i/16][i%16].nTextures; j++)
		{
			MCLY Layer;
			//First DWORD is the Texture Offset
			//memcpy(&Temp,Buffer+Change+MCINs[i].offset+ChunkHeader[i].ofsLayer+8+16*j,4);	
			Layer.textureID = textureOffsets[chunks[i/16][i%16].textures[j]];
			Layer.flags = chunks[i/16][i%16].texFlags[j];
			
			if(j > 0)
				Layer.flags = Layer.flags | 0x100;
			else
				Layer.flags = Layer.flags & 0xff;

			//Third DWORD is the Alpha Offset
			if(j == 0)
				Layer.ofsAlpha = 0;
			else
				Layer.ofsAlpha = 32 * 64 * (j - 1);

			Layer.effectID = chunks[i/16][i%16].effectID[j];

			memcpy(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsLayer + 8 + 16 * j, &Layer, sizeof(MCLY));	
		}

		lChange = 16 * (chunks[i/16][i%16].nTextures - ChunkHeader[i].nLayers);

		memcpy(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsRefs + lChange, f.getBuffer() + MCINs[i].offset + ChunkHeader[i].ofsRefs, ChunkHeader[i].ofsShadow - ChunkHeader[i].ofsRefs);

		//ChunkHeader[i].nLayers=chunks[i/16][i%16].nTextures;
		
		//Constructing Object References
		Temp = 4 * (NumWMORefs + NumDoodadRefs);
		memcpy(Buffer + Change+MCINs[i].offset+ChunkHeader[i].ofsRefs + 4 + lChange, &Temp, 4);
		memcpy(Buffer + Change+MCINs[i].offset+ChunkHeader[i].ofsRefs + 8 + lChange, ChunkDoodadRef, 4 * NumDoodadRefs);
		memcpy(Buffer + Change+MCINs[i].offset+ChunkHeader[i].ofsRefs + 8 + 4 * NumDoodadRefs + lChange, ChunkWMORef, 4 * NumWMORefs);
		int SizeAfterRef = MCNKSize + 8 - ChunkHeader[i].ofsShadow;
		memcpy(Buffer + Change+MCINs[i].offset+ChunkHeader[i].ofsRefs + 8 + Temp + lChange, f.getBuffer() + MCINs[i].offset + ChunkHeader[i].ofsShadow, SizeAfterRef);
		
		ChunkHeader[i].ofsRefs += lChange;
		lChange += 4 * (NumDoodadRefs - ChunkHeader[i].nDoodadRefs) + 4 * (NumWMORefs - ChunkHeader[i].nMapObjRefs);
		
		ChunkHeader[i].nDoodadRefs = NumDoodadRefs;
		ChunkHeader[i].nMapObjRefs = NumWMORefs;
		ChunkHeader[i].ofsShadow += lChange;
		ChunkHeader[i].ofsAlpha += lChange;

		bool NewTexture = false;
		//if(ChunkHeader[i].nLayers!=chunks[i/16][i%16].nTextures)
		//	NewTexture=true;

		//
		//Now time to construct the alpha maps
		lChange += max((int)chunks[i/16][i%16].nTextures - 1, (int)0) * 64 * 32 + 8 - ChunkHeader[i].sizeAlpha;
		Temp = max((int)chunks[i/16][i%16].nTextures - 1, (int)0) * 64 * 32;
		memcpy(Buffer + Change + MCINs[i].offset+ChunkHeader[i].ofsAlpha + 4, &Temp, 4);

		unsigned char alphaMap[64 * 32], upperNibble, lowerNibble;
		for(int j = 0; j < (int)chunks[i/16][i%16].nTextures - 1; j++)
		{
			//First thing we have to do is downsample the alpha maps before we can write them
			for(int k = 0; k < 64 * 32; k++)
			{
				lowerNibble = (unsigned char)max(min(((float)chunks[i/16][i%16].amap[j][k * 2]) / 255.0f * 15.0f + 0.5f, 15.0f), 0.0f);
				upperNibble = (unsigned char)max(min(((float)chunks[i/16][i%16].amap[j][k * 2 + 1]) / 255.0f * 15.0f + 0.5f, 15.0f), 0.0f);
				alphaMap[k] = (upperNibble << 4) + lowerNibble;
			}
			//if(NewTexture)
			//	gLog("[World of Warcraft Studio - Editor] - Adding new texture alpha layer at %x\n",Change+MCINs[i].offset+ChunkHeader[i].ofsAlpha+8+64*32*j);
			memcpy(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsAlpha + 8 + 64 * 32 * j, &alphaMap, 64 * 32);
		}
		
		ChunkHeader[i].sizeAlpha = 8 + 64 * 32 * max((int)chunks[i/16][i%16].nTextures - 1, (int)0);
		
		SizeAfterRef = MCNKSize + 8 - ChunkHeader[i].ofsLiquid;
		memcpy(Buffer + Change + MCINs[i].offset + ChunkHeader[i].ofsLiquid + lChange, f.getBuffer() + MCINs[i].offset + ChunkHeader[i].ofsLiquid, SizeAfterRef);

		ChunkHeader[i].nLayers = chunks[i/16][i%16].nTextures;
		ChunkHeader[i].ofsLiquid += lChange;
		ChunkHeader[i].ofsSndEmitters += lChange;

		memcpy(Buffer + Change + MCINs[i].offset + 8, &ChunkHeader[i], sizeof(MapChunkHeader));

		//Write the new size
		MCNKSize += lChange;
		memcpy(Buffer + Change + MCINs[i].offset + 4, &MCNKSize, 4);

		MCINs[i].offset += Change;
		MCINs[i].size += lChange;
		
		Change += lChange;
	}

	memcpy(Buffer + 0x5c, MCINs, sizeof(MCIN) * 256);

	memcpy(Buffer + 0x14, &newMapHeader, sizeof(MHDR));

	f.setBuffer(Buffer, Change + (int)f.getSize());
	f.SaveFile();
	f.close();
}

bool MapTile::GetVertex(float x, float z, Vec3D* V)
{
	float xdiff, zdiff;

	xdiff = x - xbase;
	zdiff = z - zbase;

	int xcol, ycol;
	xcol = (int)(xdiff/CHUNKSIZE);
	ycol = (int)(zdiff/CHUNKSIZE);
	if((xcol < 0) || (xcol > 15) || (ycol < 0) || (ycol > 15))
		return false;

	return chunks[ycol][xcol].GetVertex(x, z, V);
}