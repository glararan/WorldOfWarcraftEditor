#include <string>
#include <map>
#include <algorithm>
#include <cctype> 
#include "GroundEffects.h"
#include "DBCFile.h"

struct ToLower
{
    char operator() (char c) const  { return tolower(c); }
};

map<int, GroundEffectTexture> groundEffectTextureData;
map<int, GroundEffectDoodad> groundEffectDoodadData;

DBCFile	groundEffectTextureDBC("DBFilesClient\\GroundEffectTexture.dbc");
DBCFile	groundEffectDoodadDBC("DBFilesClient\\GroundEffectDoodad.dbc");

void InitGroundEffects()
{
	GroundEffectDoodad tmpGroundEffectDoodad;
	groundEffectDoodadDBC.open();

	for(DBCFile::Iterator i = groundEffectDoodadDBC.begin(); i != groundEffectDoodadDBC.end(); ++i)
	{
		tmpGroundEffectDoodad.id = i->getUInt(0);
		tmpGroundEffectDoodad.Model = i->getString(1);
		tmpGroundEffectDoodad.flags = i->getUInt(2);

		groundEffectDoodadData[tmpGroundEffectDoodad.flags] = tmpGroundEffectDoodad;
	}

	GroundEffectTexture	tmpGroundEffectTexture;
	groundEffectTextureDBC.open();

	for(DBCFile::Iterator i = groundEffectTextureDBC.begin(); i != groundEffectTextureDBC.end(); ++i)
	{
		tmpGroundEffectTexture.id = i->getUInt(0);
		tmpGroundEffectTexture.doodad[0] = i->getUInt(1);
		tmpGroundEffectTexture.doodad[1] = i->getUInt(2);
		tmpGroundEffectTexture.doodad[2] = i->getUInt(3);
		tmpGroundEffectTexture.doodad[3] = i->getUInt(4);
		tmpGroundEffectTexture.amount = i->getUInt(9);

		groundEffectTextureData[tmpGroundEffectTexture.id] = tmpGroundEffectTexture;
	}
}

const char* getGroundEffectDoodad(unsigned int effectDoodad, int amount)
{
	if(groundEffectTextureData[effectDoodad].doodad[amount] != -1)
		return groundEffectDoodadData[groundEffectTextureData[effectDoodad].doodad[amount]].Model;
	else
		return 0;
}