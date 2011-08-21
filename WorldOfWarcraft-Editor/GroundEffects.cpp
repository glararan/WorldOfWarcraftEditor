#include <string>
#include <map>
#include <algorithm>
#include <cctype> 
#include "GroundEffects.h"
#include "DBCFile.h"

using namespace std;

struct ToLower
{
    char operator() (char c) const  { return std::tolower(c); }
};

std::map<int,GroundEffectTexture>	groundEffectTextureData;
std::map<int,GroundEffectDoodad>	groundEffectDoodadData;

DBCFile	groundEffectTextureDBC("DBFilesClient\\GroundEffectTexture.dbc");
DBCFile	groundEffectDoodadDBC("DBFilesClient\\GroundEffectDoodad.dbc");

void InitGroundEffects()
{
	string Temp;
	char	*Temp2;
	GroundEffectTexture	tmpGroundEffectTexture;
	groundEffectTextureDBC.open();
	for(DBCFile::Iterator i=groundEffectTextureDBC.begin();i!=groundEffectTextureDBC.end();i++)
	{
		tmpGroundEffectTexture.id=i->getUInt(0);
		//Old Pre 1.11 Patch method
		/*Temp=i->getString(4);
		std::transform (Temp.begin(), Temp.end(), Temp.begin(), ToLower());
		Temp2=new char[strlen(Temp.c_str())+1];
		strcpy(Temp2,Temp.c_str());
		tmpGroundEffectTexture.texture=Temp2;
		
		tmpGroundEffectTexture.doodad[0]=i->getUInt(5);
		tmpGroundEffectTexture.doodad[1]=i->getUInt(6);
		tmpGroundEffectTexture.doodad[2]=i->getUInt(7);
		tmpGroundEffectTexture.doodad[3]=i->getUInt(8);*/

		//New Patch 1.11 method
		tmpGroundEffectTexture.doodad[0]=i->getUInt(1);
		tmpGroundEffectTexture.doodad[1]=i->getUInt(2);
		tmpGroundEffectTexture.doodad[2]=i->getUInt(3);
		tmpGroundEffectTexture.doodad[3]=i->getUInt(4);
		groundEffectTextureData[tmpGroundEffectTexture.id]=tmpGroundEffectTexture;
	}

	GroundEffectDoodad	tmpGroundEffectDoodad;
	groundEffectDoodadDBC.open();
	for(DBCFile::Iterator i=groundEffectDoodadDBC.begin();i!=groundEffectDoodadDBC.end();i++)
	{
		tmpGroundEffectDoodad.id=i->getUInt(0);
		tmpGroundEffectDoodad.doodadID=i->getUInt(1);
		tmpGroundEffectDoodad.Model=i->getString(2);

		groundEffectDoodadData[tmpGroundEffectDoodad.doodadID]=tmpGroundEffectDoodad;
	}
}

const char* getGroundEffectDoodad(unsigned int effectID, int DoodadNum)
{
	if(groundEffectTextureData[effectID].doodad[DoodadNum]!=-1)
		return groundEffectDoodadData[groundEffectTextureData[effectID].doodad[DoodadNum]].Model;
	else
		return 0;
}

unsigned int findEffectID(const char *tex)
{
	//Blizzard decided to remove texture names from DBC's
	return 0;
	
	for(std::map<int,GroundEffectTexture>::iterator i=groundEffectTextureData.begin();i!=groundEffectTextureData.end();i++)
	{
		const char *Tmp=i->second.texture;
		if(strcmp(Tmp,tex)==0)
			return  i->second.id;
	}
	return 0;
}